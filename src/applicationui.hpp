#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QObject>

namespace bb
{
    namespace cascades
    {
        class LocaleHandler;
    }
}

class QTranslator;

namespace Purple {
    class ApplicationUI : public QObject
    {
        Q_OBJECT

    public:
        ApplicationUI();
        virtual ~ApplicationUI() {}

    private slots:
        void onSystemLanguageChanged();
    private:
        QTranslator* m_pTranslator;
        bb::cascades::LocaleHandler* m_pLocaleHandler;

        void writeSettingsFile();
    };
}
#endif /* ApplicationUI_HPP_ */
