#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QObject>
#include "SyncNetworkManager.hpp"
#include "settings.hpp"

namespace bb
{
    namespace cascades
    {
        class LocaleHandler;
    }
}

class QTranslator;

using Purple::Settings;
using Purple::SyncNetworkManager;

class ApplicationUI : public QObject
{
    Q_OBJECT

    Q_PROPERTY( Settings *appSettings READ appSettings )
    Q_PROPERTY( SyncNetworkManager *networkManager READ networkManager )
public:
    ApplicationUI();
    virtual ~ApplicationUI() {}
    SyncNetworkManager *networkManager();
    Settings           *appSettings();
private slots:
    void onSystemLanguageChanged();
private:
    QTranslator* m_pTranslator;
    bb::cascades::LocaleHandler* m_pLocaleHandler;

    SyncNetworkManager *m_networkManager;
    Settings *m_appSettings;
};

#endif /* ApplicationUI_HPP_ */
