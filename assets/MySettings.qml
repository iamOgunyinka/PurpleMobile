import bb.cascades 1.0

Container {
    property string  thumbnailsQuality;
    property string  appTheme;
    property int     maxResults;
    property string  safeSearch;
    property string  fileExists;
    
    topPadding: 30
    leftPadding: 30
    rightPadding: 30
    DropDown {
        id: appThemeOption
        title: "Theme"
        options: [
            Option {
                text: "Light"
            },
            Option {
                text: "Dark"
            }
        ]
        onSelectedOptionChanged: {
            appTheme = selectedOption.text
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
            },
            Option {
                text: "Moderate"
            },
            Option {
                text: "Strict"
            }
        ]
        onCreationCompleted: {
            safeSearchOption.setSelectedIndex(0)
        }
        onSelectedOptionChanged: {
            safeSearch = selectedOption.text
        }
    }
    DropDown {
        id: thumbnailsQualityOption            
        title: "Thumbnails Quality"
        onSelectedOptionChanged: {
            thumbnailsQuality = selectedOption.text
        }
        onCreationCompleted: {
            thumbnailsQualityOption.setSelectedIndex(0)
        }
        options: [
            Option {
                text: "Default"
            },
            Option {
                text: "Medium"
            },
            Option {
                text: "High"
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
            value: 20
            onValueChanged: {
                maximumResultsLabel.text = "Results per search: " + Math.round( value )
                maxResults = Math.round( value )
            }
        }
    }
    Divider {}
    
    Container {
        Label {
            text: "If file already exist"
        }
        DropDown {
            id: fileExistsOption
            options: [
                Option {
                    text: "Cancel"
                },
                Option {
                    text: "Overwrite"
                },
                Option {
                    text: "Rename"
                }
            ]
            onCreationCompleted: {
                fileExistsOption.setSelectedIndex(1)
            }
            onSelectedOptionChanged: {
                fileExists = selectedOption.text                
            }
        }
    }
}