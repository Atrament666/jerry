/*
* Copyright (c) 2001  Shane Hudson
* Copyright (C) 2014  Fulvio Benini

* This file is part of Scid (Shane's Chess Information Database).
*
* Scid is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation.
*
* Scid is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Scid.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "gfile.h"
#include "mfile.h"
#include "misc.h"

namespace scid {

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// GFile::Init(): Initialise the GFile.
//
void
GFile::Init ()
{
    FileName[0] = 0;
    Handle = NULL;
    FileMode = FMODE_None;
    Offset = 0;
    NumBlocks = 0;
    LastBlockSize = 0;
    CurrentBlock.blockNum = -1;
    CurrentBlock.dirty = false;
    CurrentBlock.length = 0;
}

GFile::~GFile() {
    FlushAll();
    if (Handle != NULL) delete Handle;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// GFile::Close(): close the GFile.
errorT
GFile::Close ()
{
    if (Handle == NULL) { return ERROR_FileNotOpen; }
    if (CurrentBlock.dirty  &&  FileMode != FMODE_ReadOnly) {
        if (Flush (&CurrentBlock) != OK) { return ERROR_FileWrite; }
    }

    delete Handle;
    Init();
    return OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// GFile::Create():
//      Create a new gfile. The parameter "fmode" can be writeonly or
//      both, but it makes no sense for it to be readonly.
//
errorT
GFile::Create (const char * filename, fileModeT fmode)
{
    if (Handle != NULL) { return ERROR_FileInUse; }
    sprintf (FileName, "%s%s", filename, GFILE_SUFFIX);
    FileMode = fmode;
    Handle = new MFile;
    if (Handle->Create (FileName, fmode) != OK) {
        Close();
        return ERROR_FileOpen;
    }
    return OK;
}

errorT
GFile::CreateMemoryOnly ()
{
    if (Handle != NULL) { return ERROR_FileInUse; }
    FileMode = FMODE_Both;
    Handle = new MFile;

    return OK;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// GFile::Open():
//      Open a gfile for reading, writing, or both.
//
errorT
GFile::Open (const char * filename, fileModeT fmode)
{
    if (Handle != NULL) { return ERROR_FileInUse; }
    FileMode = fmode;
    const char* suffix = GFILE_SUFFIX;
    sprintf (FileName, "%s%s", filename, suffix);
    Handle = new MFile;
    if (Handle->Open (FileName, fmode) != OK) {
        Close();
        return ERROR_FileOpen;
    }

    uint fsize = fileSize (filename, suffix);
    NumBlocks = (fsize + GF_BLOCKSIZE - 1) / GF_BLOCKSIZE;
    if (NumBlocks > 0) {
        LastBlockSize = (fsize % GF_BLOCKSIZE);
        if (LastBlockSize == 0) { LastBlockSize = GF_BLOCKSIZE; }
    }
    return OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// GFile::Flush():
//      Flush any blocks that have been modified to the disk.
//
errorT
GFile::Flush (gfBlockT * blk)
{
    if (Handle == NULL) { return ERROR_FileNotOpen; }
    if (FileMode == FMODE_ReadOnly) { return ERROR_FileMode; }
    if (!blk->dirty) {
        // File is clean, no need to write anything.
        return OK;
    }
    if (blk->blockNum == -1) {
        // No blocks in the file yet.
        return OK;
    }
    uint filePos = blk->blockNum * GF_BLOCKSIZE;
    if (Offset != filePos) {
        if (Handle->Seek(filePos) != OK) { return ERROR_FileSeek; }
        Offset = filePos;
    }
    uint numBytes = GF_BLOCKSIZE;
    if (blk->blockNum == (int)NumBlocks - 1) {
        // Last block, so only write "length" bytes.
        numBytes = blk->length;
    }
    if (Handle->WriteNBytes ((const char *)blk->data, numBytes) != OK) {
        return ERROR_FileWrite; 
    }
    if (FileMode == FMODE_Both) { Handle->Flush(); }
    Offset += numBytes;
    blk->dirty = false;
    return OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// GFile::Fetch():
//      Fetch a single block from the file.
//
errorT
GFile::Fetch (gfBlockT * blk, int blkNum)
{
    if (Handle == NULL) { return ERROR_FileNotOpen; }
    if (blk->dirty  &&  FileMode != FMODE_ReadOnly) { Flush(blk); }
    uint filePos = blkNum * GF_BLOCKSIZE;
    if (Offset != filePos) {
        if (Handle->Seek(filePos) != OK) { return ERROR_FileSeek; }
        Offset = filePos;
    }
    uint numBytes = GF_BLOCKSIZE;
    if (blkNum == (NumBlocks - 1)) { 
        // Last block, so its size is LastBlockSize
        numBytes = LastBlockSize;
    }
    if (Handle->ReadNBytes ((char *)blk->data, numBytes) != OK) {
        return ERROR_FileRead; 
    }
    Offset += numBytes;
    blk->dirty = false;
    blk->blockNum = blkNum;
    blk->length = numBytes;
    return OK;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// GFile::AddGame():
//      Add a game record to the file. It is added to the end of the
//      last block, or a new block is added if the record will not
//      fit in the last block.
//
errorT
GFile::AddGame (ByteBuffer * bb, uint * offset)
{
    *offset = 0;
    if (Handle == NULL) { return ERROR_FileNotOpen; }
    if (FileMode == FMODE_ReadOnly) { return ERROR_FileMode; }
    if (NumBlocks == 0) { // First block for this file
        NumBlocks++;
        CurrentBlock.blockNum = 0;
        CurrentBlock.length = 0;
    } else {
        // Either add to the last block, or make a new block:
        
        if (LastBlockSize + bb->GetByteCount() > GF_BLOCKSIZE) {
            // Need a new block!
            
            if (Flush(&CurrentBlock) != OK) { return ERROR_FileWrite; }
            CurrentBlock.blockNum = NumBlocks++;
            CurrentBlock.length = 0;
        } else {
            // It will fit in the last block. Fetch it:
            
            if (CurrentBlock.blockNum != (int) NumBlocks - 1) {
                Fetch (&CurrentBlock, NumBlocks - 1);
            }
        }
    }
    
    // Now, CurrentBlock contains the block the game will be added to.
    ASSERT (CurrentBlock.length + bb->GetByteCount() <= GF_BLOCKSIZE);

    bb->CopyTo (&(CurrentBlock.data[CurrentBlock.length]));
    *offset = CurrentBlock.blockNum * GF_BLOCKSIZE + CurrentBlock.length;
    CurrentBlock.length += bb->GetByteCount();
    LastBlockSize = CurrentBlock.length;
    CurrentBlock.dirty = true;
    return OK;
}

}

//////////////////////////////////////////////////////////////////////
//  EOF: gfile.cpp
//////////////////////////////////////////////////////////////////////

