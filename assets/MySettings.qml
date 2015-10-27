import bb.cascades 1.0

Container {
    property string  thumbnailsQuality;
    property string  appTheme;
    property string  youtubeUrl;
    property string  apiKey;
    property int     maxResults;
    property string  safeSearch;
    
    attachedObjects: [
        ActionItem {
            title: "Back"
            onTriggered: {
            
            }
            ActionBar.placement: ActionBarPlacement.OnBar
        }    
    ]
    DropDown {
        id: appThemeOption
        title: "Theme"
        options: [
            Option {
                text: "Light"
                value: 0
            },
            Option {
                text: "Dark"
                value: 1
            }
        ]
        onSelectedIndexChanged: {
            switch ( selectedIndex ){
                case 0:{
                        appTheme = "Dark"
                        break;
                }
            case 1: {
                    appTheme = "Light"
                    break;
            }
        default :
            break;
            }
        }
        onCreationCompleted: {
            appThemeOption.setSelectedIndex( 0 )
        }
    }
    Divider {}
    
    DropDown {
        id: safeSearchOption
        title: "Safe Search"
        options: [
            Option {
                text: "None"
                value: "none"
            },
            Option {
                text: "Moderate"
                value: "moderate"
            },
            Option {
                text: "Strict"
                value: "strict"
            }
        ]
    }
    DropDown {
        id: thumbnailsQualityOption            
        title: "Thumbnails Quality"
        options: [
            Option {
                text: "Default"
                value: "default"
            },
            Option {
                text: "Medium"
                value: "medium"
            },
            Option {
                text: "High"
                value: "high"
            }
        ]
    }
    Divider {}
    Container {
        Label {
            id: maximumResultsLabel
            text: "Results per search"
        }
        Slider {
            fromValue: 5
            toValue: 50
            onValueChanged: {
                maximumResultsLabel.text = "Results per search: " + Math.round( value )
                maxResults = Math.round( value )
            }
        }
    }
}