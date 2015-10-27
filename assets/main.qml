import bb.cascades 1.0
import bb.system 1.0
import bb.data 1.0

TabbedPane {
    id: root
    showTabsOnActionBar: true
    Menu.definition: MenuDefinition {
        settingsAction: SettingsActionItem {
            onTriggered: {
                settingsSheet.open()
            }
        }
    }
    attachedObjects: [
        MySettings {
        },
        SystemDialog {
            id: errorDialog
            title: "Error"
            body: ""
        },
        Sheet {
            id: settingsSheet
            Page {
                id: settingsPage
                titleBar: TitleBar {
                    title: "Settings"
                }
                MySettings {
                    onAppThemeChanged: {
//                        appTheme = _tools.appSettings.appTheme
                    }
                    onMaxResultsChanged: {
//                        maxResults = _tools.appSettings.maxResult
                    }
                }
            }
        }
    ]
    Tab {
        id: homeTab
        title: "Home"
        imageSource: "asset:///ad_search.png"
        
        function searchResultObtained( result )
        {
            myResult.setText( result )
        }
        
        function errorGotten( error )
        {
            errorDialog.body = error
            errorDialog.show()
        }

        function processRequest( query )
        {
            var m_url = _tools.appSettings.youtubeUrl
            console.log( m_url )
            m_url = m_url + "&q=" + _tools.networkManager.toPercentage( query );
            m_url = m_url + "&maxResults=" + _tools.networkManager.maxResults
            m_url = m_url + "&key=" + _tools.networkManager.apiKey;
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
//            _tools.networkManager.errorOccurred.connect( errorGotten )            
            _tools.networkManager.finished.connect( searchResultObtained )
            _tools.networkManager.networkError.connect( errorGotten )
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