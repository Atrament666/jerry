#-------------------------------------------------
#
# Project created by QtCreator 2015-10-11T20:38:05
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

QT       += printsupport
QT       += svg
QT       -= gui

TARGET = Jerry
CONFIG   -= console
CONFIG   += app_bundle
CONFIG   += c++14
#CONFIG += object_parallel_to_source



TEMPLATE = app


SOURCES += main.cpp \
    dialogs/tab_board_style.cpp \
    dialogs/tab_font_style.cpp \
    model/font_style.cpp \
    profile/profile.cpp \
    scid/bytebuf.cpp \
    scid/date.cpp \
    scid/dstring.cpp \
    scid/game.cpp \
    scid/gfile.cpp \
    scid/matsig.cpp \
    scid/mfile.cpp \
    scid/misc.cpp \
    scid/namebase.cpp \
    scid/pgnparse.cpp \
    scid/position.cpp \
    scid/stored.cpp \
    scid/textbuf.cpp \
    scid_test.cpp \
    uci/uci_controller.cpp \
    uci/uci_worker.cpp \
    uci/engine_info.cpp \
    funct.cpp \
    main_window.cpp \
    viewController/boardviewcontroller.cpp \
    model/game_model.cpp \
    viewController/piece_images.cpp \
    dialogs/dialog_promotion.cpp \
    viewController/moveviewcontroller.cpp \
    dialogs/dialog_nextmove.cpp \
    dialogs/dialog_plaintext.cpp \
    controller/mode_controller.cpp \
    dialogs/dialog_guioptions.cpp \
    dialogs/dialog_enterposition.cpp \
    viewController/colorstyle.cpp \
    viewController/engineview.cpp \
    model/engine.cpp \
    model/internalengine.cpp \
    dialogs/dialog_engines.cpp \
    dialogs/dialog_engineoptions.cpp \
    model/engine_option.cpp \
    dialogs/dialog_about.cpp \
    various/messagebox.cpp \
    controller/edit_controller.cpp \
    controller/file_controller.cpp \
    dialogs/dialog_newgame.cpp \
    dialogs/dialog_browseheaders.cpp \
    dialogs/dialog_editheaders.cpp \
    dialogs/dialog_gameanalysis.cpp \
    various/resource_finder.cpp \
    viewController/on_off_button.cpp \
    viewController/chessboard.cpp \
    viewController/enterposboard.cpp \
    viewController/pickcolorboard.cpp \
    various/helper.cpp \
    viewController/database_index_model.cpp \
    dialogs/dialog_database.cpp \
    dialogs/dialog_database_help.cpp \
    model/search_pattern.cpp \
    dialogs/dialog_search.cpp \
    dialogs/tab_header_search.cpp

HEADERS += \
    dialogs/tab_board_style.h \
    dialogs/tab_font_style.h \
    model/font_style.h \
    profile/profile.h \
    scid/attacks.h \
    scid/bytebuf.h \
    scid/common.h \
    scid/date.h \
    scid/dstring.h \
    scid/error.h \
    scid/fastgame.h \
    scid/filebuf.h \
    scid/fullmove.h \
    scid/game.h \
    scid/gfile.h \
    scid/hash.h \
    scid/indexentry.h \
    scid/matsig.h \
    scid/mfile.h \
    scid/misc.h \
    scid/movelist.h \
    scid/naglatex.h \
    scid/nagtext.h \
    scid/namebase.h \
    scid/pgnparse.h \
    scid/position.h \
    scid/sqlist.h \
    scid/sqmove.h \
    scid/sqset.h \
    scid/stored.h \
    scid/textbuf.h \
    scid/tokens.h \
    scid_test.h \
    uci/uci_worker.h \
    uci/uci_controller.h \
    uci/engine_info.h \
    funct.h \
    main_window.h \
    viewController/boardviewcontroller.h \
    model/game_model.h \
    viewController/piece_images.h \
    dialogs/dialog_promotion.h \
    viewController/moveviewcontroller.h \
    dialogs/dialog_nextmove.h \
    dialogs/dialog_plaintext.h \
    controller/mode_controller.h \
    dialogs/dialog_guioptions.h \
    dialogs/dialog_enterposition.h \
    viewController/colorstyle.h \
    viewController/engineview.h \
    model/engine.h \
    model/internalengine.h \
    dialogs/dialog_engines.h \
    dialogs/dialog_engineoptions.h \
    model/engine_option.h \
    dialogs/dialog_about.h \
    various/messagebox.h \
    controller/edit_controller.h \
    controller/file_controller.h \
    dialogs/dialog_newgame.h \
    dialogs/dialog_browseheaders.h \
    dialogs/dialog_editheaders.h \
    dialogs/dialog_gameanalysis.h \
    various/resource_finder.h \
    viewController/on_off_button.h \
    viewController/chessboard.h \
    viewController/enterposboard.h \
    viewController/pickcolorboard.h \
    various/helper.h \
    chess/arrow.h \
    chess/colored_field.h \
    viewController/database_index_model.h \
    dialogs/dialog_database.h \
    dialogs/dialog_database_help.h \
    model/search_pattern.h \
    dialogs/dialog_search.h \
    dialogs/tab_header_search.h

RESOURCES += \
    resources.qrc

DISTFILES += \
    scid/universalchardet/Big5Freq.tab \
    scid/universalchardet/EUCKRFreq.tab \
    scid/universalchardet/EUCTWFreq.tab \
    scid/universalchardet/GB2312Freq.tab \
    scid/universalchardet/JISFreq.tab
