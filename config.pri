# Config.pri file version 2.0. Auto-generated by IDE. Any changes made by user will be lost!
BASEDIR = $$quote($$_PRO_FILE_PWD_)

device {
    CONFIG(debug, debug|release) {
        profile {
            CONFIG += \
                config_pri_assets \
                config_pri_source_group1
        } else {
            CONFIG += \
                config_pri_assets \
                config_pri_source_group1
        }

    }

    CONFIG(release, debug|release) {
        !profile {
            CONFIG += \
                config_pri_assets \
                config_pri_source_group1
        }
    }
}

simulator {
    CONFIG(debug, debug|release) {
        !profile {
            CONFIG += \
                config_pri_assets \
                config_pri_source_group1
        }
    }
}

config_pri_assets {
    OTHER_FILES += \
        $$quote($$BASEDIR/assets/DownloadManager.qml) \
        $$quote($$BASEDIR/assets/Help.qml) \
        $$quote($$BASEDIR/assets/ad_search.png) \
        $$quote($$BASEDIR/assets/download.png) \
        $$quote($$BASEDIR/assets/downloads_icon.png) \
        $$quote($$BASEDIR/assets/exit.png) \
        $$quote($$BASEDIR/assets/logo.png) \
        $$quote($$BASEDIR/assets/main.qml) \
        $$quote($$BASEDIR/assets/property.png) \
        $$quote($$BASEDIR/assets/refresh.png) \
        $$quote($$BASEDIR/assets/search.png) \
        $$quote($$BASEDIR/assets/video.png)
}

config_pri_source_group1 {
    SOURCES += \
        $$quote($$BASEDIR/src/DownloadManager.cpp) \
        $$quote($$BASEDIR/src/DownloadsDataModel.cpp) \
        $$quote($$BASEDIR/src/MyIndexMapper.cpp) \
        $$quote($$BASEDIR/src/Stream.cpp) \
        $$quote($$BASEDIR/src/SyncNetworkManager.cpp) \
        $$quote($$BASEDIR/src/ThumbnailSaver.cpp) \
        $$quote($$BASEDIR/src/YTDownloadExtractor.cpp) \
        $$quote($$BASEDIR/src/YTDownloadExtractor_test.cpp) \
        $$quote($$BASEDIR/src/applicationui.cpp) \
        $$quote($$BASEDIR/src/main.cpp)

    HEADERS += \
        $$quote($$BASEDIR/src/DownloadManager.hpp) \
        $$quote($$BASEDIR/src/DownloadsDataModel.hpp) \
        $$quote($$BASEDIR/src/MyIndexMapper.hpp) \
        $$quote($$BASEDIR/src/Stream.hpp) \
        $$quote($$BASEDIR/src/SyncNetworkManager.hpp) \
        $$quote($$BASEDIR/src/ThumbnailSaver.hpp) \
        $$quote($$BASEDIR/src/YTDownloadExtractor.hpp) \
        $$quote($$BASEDIR/src/applicationui.hpp)
}

CONFIG += precompile_header

PRECOMPILED_HEADER = $$quote($$BASEDIR/precompiled.h)

lupdate_inclusion {
    SOURCES += \
        $$quote($$BASEDIR/../src/*.c) \
        $$quote($$BASEDIR/../src/*.c++) \
        $$quote($$BASEDIR/../src/*.cc) \
        $$quote($$BASEDIR/../src/*.cpp) \
        $$quote($$BASEDIR/../src/*.cxx) \
        $$quote($$BASEDIR/../assets/*.qml) \
        $$quote($$BASEDIR/../assets/*.js) \
        $$quote($$BASEDIR/../assets/*.qs)

    HEADERS += \
        $$quote($$BASEDIR/../src/*.h) \
        $$quote($$BASEDIR/../src/*.h++) \
        $$quote($$BASEDIR/../src/*.hh) \
        $$quote($$BASEDIR/../src/*.hpp) \
        $$quote($$BASEDIR/../src/*.hxx)
}

TRANSLATIONS = $$quote($${TARGET}.ts)
