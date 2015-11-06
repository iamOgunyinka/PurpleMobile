import bb.cascades 1.2
import bb.system 1.0
import bb.data 1.0
import purple.searchModel 1.0

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
        SearchDataModel {
            id: youtubeManager
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
                    
                    appTheme: youtubeManager.appTheme
                    safeSearch: youtubeManager.safeSearch
                    fileExist: youtubeManager.fileExistPolicy
                    thumbnails: youtubeManager.thumbnailsQuality
                    maxResults: youtubeManager.maxResult
                    
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
                    onSafeSearchChanged: {
                        youtubeManager.safeSearch = value
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
            resultDropDown.visible = true
        
        }
        
        function errorGotten(error) {
            errorDialog.body = error
            errorDialog.show()
        }
        
        function processRequest(query)
        {
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
                ListView {
                    id: listView
                    dataModel: youtubeManager
                    listItemComponents: [
                        ListItemComponent {
                            type: "title"
                            Header {
                                title: "Results"
                            }
                        },
                        ListItemComponent {
                            type: "result"
                            StandardListItem {
                                title: ListItemData.title
                                imageSource: ListItemData.thumbnails
                                status: ListItemData.owner
                                description: ListItemData.details
                                
                                contextActions: [
                                    ActionSet {
                                        ActionItem {
                                            title: "Downloads"
                                            onTriggered: {
                                                console.log( "Downloaded" );
                                            }
                                        }
                                    },
                                    ActionSet {
                                        ActionItem {
                                            title: "Details"
                                            onTriggered: {
                                                console.log( "Details" )
                                            }
                                        }
                                    },
                                    ActionSet {
                                        ActionItem {
                                            title: "Copy URL"
                                            onTriggered: {
                                                console.log( "URL copied" )
                                            }
                                        }
                                    }
                                ]
                            }
                        }
                    ]
                    function itemType( data, indexPath )
                    {
                        if( indexPath.length == 2 ) return "result"
                        if( indexPath.length == 1 ) return "title"
                        return ""
                    }
                }
            }
        }
        
        onCreationCompleted: {
            youtubeManager.setProjectFile( "app/data/assets/settings.json" );
            youtubeManager.error.connect( homeTab.errorGotten )
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