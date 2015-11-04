import bb.cascades 1.2
import bb.system 1.0
import bb.data 1.0
import purple.searchModel 1.0
import purple.youtube 1.0

TabbedPane {
    id: root
    showTabsOnActionBar: true
    Menu.definition: MenuDefinition {
        settingsAction: SettingsActionItem {
            onTriggered: {
                settingsSheet.updateContent()
                settingsSheet.open();
            }
        }
    }
    attachedObjects: [
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
            function updateContent()
            {
                mySheetSettings.appTheme = youtubeManager.appTheme
                mySheetSettings.fileExist = youtubeManager.fileExistPolicy
                mySheetSettings.maxResults = youtubeManager.maxResult
                mySheetSettings.safeSearch = youtubeManager.safeSearch
                mySheetSettings.thumbnails = youtubeManager.thumbnailsQuality
            }
            
            id: settingsSheet
            Page { 
                titleBar: TitleBar {
                    title: "Settings"
                }
                onCreationCompleted: {
                    settingsSheet.updateContent()
                }
                content: MySettings {
                    id: mySheetSettings
                    onAppThemeChanged: {
                        youtubeManager.appTheme = value
                    }
                    onFileExistChanged: {
                        youtubeManager.appTheme = value
                    }
                    onThumbnailsQualityChanged: {
                        youtubeManager.thumbnailsQuality = value
                    }
                    onMaxResultChanged: {
                        youtubeManager.maxResult = value
                    }
                }
                actions: [
                    ActionItem {
                        title: "Close"
                        ActionBar.placement: ActionBarPlacement.OnBar
                        onTriggered: {
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
            
        }
        
        function errorGotten(error) {
            loadingIndicator.stop()
            errorDialog.body = error
            errorDialog.show()
        }
        
        function processRequest(query)
        {
            loadingIndicator.running = true
            loadingIndicator.start()            
            youtubeManager.search( query )
        }
        
        function processSearchOrDownload( searchText )
        {
            resultDropDown.visible = false
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
                
                Container {
                    ListView {
                        id: listView
                        dataModel: searchDataModel
                        listItemComponents: [
                            ListItemComponent {
                                type: ""
                                Header {
                                    title: ListItem.value
                                }
                            },
                            ListItemComponent {
                                type: ""
                                StandardListItem {
                                    title: ListItem.title
                                    imageSource: ListItem.thumbnails
                                    status: ListItem.owner
                                    description: ListItem.description
                                }
                            }
                        ]
                    }
                }
            }
        }
        onCreationCompleted: {
            youtubeManager.setProjectFile( "app/data/assets/settings.json" );
        }
        attachedObjects: [
            CppSearchDataModel {
                id: searchDataModel
                onError: {
                    homeTab.errorGotten( what )
                }
            },
            CppYoutube {
                id: youtubeManager
                onError: {
                    homeTab.errorGotten( what )
                }
                onFinished: {
                    searchDataModel.setSource( data )
                }
            }
        ]
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