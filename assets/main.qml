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
            NavigationPane {
                Page { 
                    titleBar: TitleBar {
                        title: "Settings"
                    }
                    MySettings {} }
            }
        }
    ]
    Tab {
        id: homeTab
        title: "Home"
        imageSource: "asset:///ad_search.png"

        function searchResultObtained(result) {
            loadingIndicator.running = false
            myResult.setText(result)
        }

        function errorGotten(error) {
            errorDialog.body = error
            errorDialog.show()
        }

        function processRequest(query) {
            loadingIndicator.running = true
            var m_url = cpptool_settings.youtubeUrl
            console.log( m_url );
            m_url = m_url + "&q=" + cpptool_network.toPercentageEncoding(query)
            m_url = m_url + "&maxResults=" + cpptool_settings.maxResult
            m_url = m_url + "&key=" + cpptool_settings.apiKey;
            
            cpptool_network.sendRequest( m_url )
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
                                    txtSearch.setHintText( "Search Video, Channels or Playlists" )
                                    break;
                                }
                            case 1:
                                {
                                    txtSearch.setHintText( "Enter youtube URL" )
                                    break;
                                }
                            case 2:
                                {
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
                    
                    TextField {
                        id: txtSearch
                        input {
                            submitKey: SubmitKey.Search
                            onSubmitted: {
                                myResult.setText("")
                                if( txtSearch.text.valueOf() == "".valueOf() ){
                                    myResult.setText( "Empty Search" )
                                } else {
                                    homeTab.processRequest( txtSearch.text )
                                }
                            }
                        }
                    }
                    
                    Button {
                        imageSource: "asset:///search.png"
                        maxWidth: 1
                        onClicked: {
                            myResult.setText("")
                            if( txtSearch.text.valueOf() == "".valueOf() ){
                                myResult.setText( "Empty Search" )
                            } else {
                                homeTab.processRequest( txtSearch.text )
                            }
                        }
                    }
                }
                
                DropDown {
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
                ScrollView {
                    Label {
                        id: myResult
                    }
                }
            }
        }
        onCreationCompleted: {
            cpptool_network.finished.connect( homeTab.searchResultObtained )
            cpptool_network.errorOccurred.connect( homeTab.errorGotten )
            cpptool_network.networkError.connect( homeTab.errorGotten )
        }
    }
    
    Tab {
        id: downloadsTab
        title: "Downloads"
        imageSource: "asset:///download.png"
        Page {
            titleBar: TitleBar {
                kind: TitleBarKind.Segmented
                options: [
                    Option {
                        text: "In Progress"
                        value: 0
                    },
                    Option {
                        text: "Stopped"
                        value: 1
                    },
                    Option {
                        text: "Completed"
                        value: 2
                    }
                ]
                id: downloadsTitleBar
                title: "Downloads"
            }
            id: downloadPageTab
            Container {
                ListView {
                    dataModel: XmlDataModel {
                        //source: "asset:///project_file.json"
                    }
                }
            }
        }
    }
}