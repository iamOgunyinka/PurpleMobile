import bb.cascades 1.0
import bb.system 1.0
import bb.data 1.0
import purple.network 1.0
import purple.settings 1.0

TabbedPane {
    id: root
    showTabsOnActionBar: true
    Menu.definition: MenuDefinition {
        settingsAction: SettingsActionItem {
            onTriggered: {
                settingsSheet.open();
            }
        }
    }
    attachedObjects: [
        CppSettings {
            id: cpptool_settings
        },
        CppNetworkManager {
            id: cpptool_network
        },
        ActivityIndicator {
            id: loadingIndicator
            running: false
        },
        
        SystemDialog {
            id: errorDialog
            title: "Error"
            body: ""
        },
        Sheet {
            id: settingsSheet
            Page { 
                titleBar: TitleBar {
                    title: "Settings"
                }
                content: MySettings {} 
                actions: [
                    ActionItem {
                        title: "Close"
                        ActionBar.placement: ActionBarPlacement.OnBar
                        onTriggered: {
                            //TODO --> Write new settings to file
                            settingsSheet.close()
                        }
                    }
                ]
            }
        }
    ]
    Tab {
        id: homeTab
        title: "Home"
        imageSource: "asset:///ad_search.png"
        
        function searchResultObtained(result) {
            loadingIndicator.running = false
            loadingIndicator.stop()
            resultDropDown.visible = true
            
            myResult.text = result
        }
        
        function errorGotten(error) {
            loadingIndicator.stop()
            errorDialog.body = error
            errorDialog.show()
        }
        
        function download( _url )
        {
            cpptool_network.sendRequest( _url );
        }
        
        function processRequest(query)
        {
            loadingIndicator.running = true
            loadingIndicator.start()
            
            cpptool_settings.setProjectFile( dirPath.settings );
            var m_url = cpptool_settings.youtubeUrl
            console.log( m_url ) 
            
            m_url = m_url + "&q=" + cpptool_network.toPercentageEncoding(query)
            m_url = m_url + "&maxResults=" + cpptool_settings.maxResult
            m_url = m_url + "&key=AIzaSyBhl_zBnEEv_xiIukkMpz8ayoiwT1UdfQk"
            
            download( m_url )
        }
        
        function processSearchOrDownload( searchText )
        {
            resultDropDown.visible = false
            myResult.setText("")
            if( searchText.valueOf() == "".valueOf() ) return;
            
            switch ( segmentedHomePage.selectedIndex ){
                case 0: case 1:
                    {
                        processRequest( searchText )
                        break;
                    }
                case 2:
                    {
                        downloadManager.startNewDownload( searchText )
                        break;
                    }
                default : break;
            }
        }
        
        Page {
            titleBar: TitleBar {
                id: homepageTitleBar
                title: "Purple"
                acceptAction: ActionItem {
                    title: "Close"
                    onTriggered: {
                        Application.minimize()
                    }
                }
            }
            id: homepageTab
            attachedObjects: [
                Option {
                    id: simple_search_option
                    text: "Simple"
                    value: 0
                },
                Option {
                    id: advanced_search_option
                    text: "Youtube URL"
                    value: 1
                },
                Option {
                    id: direct_url_option
                    text: "Others"
                    value: 2
                }
            ]
            Container {
                topPadding: 20
                SegmentedControl {
                    id: segmentedHomePage
                    options: [ 
                    simple_search_option, 
                    advanced_search_option,
                    direct_url_option
                    ]
                    onSelectedOptionChanged: {
                        var value = segmentedHomePage.selectedIndex
                        switch ( value )
                        {
                            case 0:
                                {
                                    searchButton.imageSource = "asset:///search.png"
                                    txtSearch.setHintText( "Search Video, Channels or Playlists" )
                                    break;
                                }
                            case 1:
                                {
                                    searchButton.imageSource = "asset:///ad_search.png"
                                    txtSearch.setHintText( "Enter youtube URL" )
                                    break;
                                }
                            case 2:
                                {
                                    searchButton.imageSource = "asset:///download.png"
                                    txtSearch.setHintText( "Enter URL for any website" )
                                    break;
                                }
                            default :
                                break;
                        }
                    }
                    onCreationCompleted: {
                        segmentedHomePage.setSelectedIndex(0)
                    }
                }
                
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    leftPadding: 20
                    rightPadding: 20
                    attachedObjects: [
                        DropDown {
                            id: resultDropDown
                            title: "In results, show only"
                            options: [
                                Option {
                                    text: "Videos"
                                    value: "youtube#video"
                                },
                                Option {
                                    text: "Channels"
                                    value: "youtube#channels"
                                },
                                Option {
                                    text: "Playlist"
                                    value: "youtube#channels"
                                }
                            ]
                            onSelectedOptionChanged: {
                                var options = selectedOption.value
                                switch ( selectedIndex )
                                {
                                    case "youtube#video": break;
                                    case "youtube#channels": break;
                                    case "youtube#playlist": break;
                                    default: break;
                                }
                            }
                        }
                    ]
                    TextField {
                        id: txtSearch
                        input {
                            submitKey: SubmitKey.Search
                            onSubmitted: {
                                homeTab.processSearchOrDownload( txtSearch.text )
                            }
                        }
                    }
                    
                    Button {
                        id: searchButton
                        imageSource: "asset:///search.png"
                        maxWidth: 1
                        onClicked: {
                            homeTab.processSearchOrDownload( txtSearch.text );
                        }
                    }
                }
                
                ScrollView {
                    Label {
                        id: myResult
                    }
                }
            }
        }
        onCreationCompleted: {
            cpptool_network.finished.connect( homeTab.searchResultObtained )
            cpptool_network.networkError.connect( homeTab.errorGotten )
            cpptool_network.errorOccurred.connect( homeTab.errorGotten )
            cpptool_settings.errorOccurred.connect( homeTab.errorGotten )
        }
    }
    
    Tab {
        id: downloadsTab
        title: "Downloads"
        imageSource: "asset:///download.png"
        content: Page {
            titleBar: TitleBar {
                kind: TitleBarKind.Segmented
                options: [
                    Option {
                        text: "All"
                        value: 0
                    },
                    Option {
                        text: "Completed"
                        value: 1
                    },
                    Option {
                        text: "Stopped"
                        value: 2
                    }
                ]
                onSelectedOptionChanged: {
                    var value = selectedOption.value
                    switch ( value ){
                        case 0: break;
                        case 1: {
                            downloadManager.showCompletedDownloads(); 
                            break;
                        }
                        case 2: {
                            downloadManager.showStoppedDownloads();
                            break;
                        }
                    }
                }
                id: downloadsTitleBar
                title: "Downloads"
            }
            DownloadManager {
                id: downloadManager
            }
        }
    }
}