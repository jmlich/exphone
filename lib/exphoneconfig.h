#ifndef EXPHONECONFIG_H
#define EXPHONECONFIG_H

#include <QObject>
#include <QVariant>
#include <QDate>

#ifdef MER_EDITION_SAILFISH
class MDConfGroup;
#else
#include <QSettings>
#endif
class QQmlEngine;
class QJSEngine;

#define OPTION(key, name, setter, def, type, gettertype, settertype)  \
    Q_PROPERTY(type name READ name WRITE setter NOTIFY name##Changed) \
    type name() const                                                 \
    { return value(key, def).gettertype(); }                          \
    void setter(settertype value)                                     \
    { setValue(key, value, &ExphoneConfig::name##Changed); }         \
    Q_SIGNAL void name##Changed();

#define BOOL_OPTION(key, name, setter, def)   \
    OPTION(key, name, setter, def, bool, toBool, bool)

#define INT_OPTION(key, name, setter, def)    \
    OPTION(key, name, setter, def, int, toInt, int)

#define UINT_OPTION(key, name, setter, def)   \
    OPTION(key, name, setter, def, uint, toInt, uint)

#define INT64_OPTION(key, name, setter, def)  \
    OPTION(key, name, setter, def, qint64, toLongLong, qint64)

#define STRING_OPTION(key, name, setter, def) \
    OPTION(key, name, setter, def, QString, toString, const QString &)

#define ENUM_OPTION(key, name, setter, type, def)                               \
    Q_PROPERTY(type name READ name WRITE setter NOTIFY name##Changed)           \
    type name() const                                                           \
    { return static_cast<type>(value(key, def).toInt()); }                      \
    void setter(type value)                                                     \
    { setValue(key, static_cast<int>(value), &ExphoneConfig::name##Changed); } \
    Q_SIGNAL void name##Changed();


class ExphoneConfig : public QObject
{
    Q_OBJECT

    ExphoneConfig(QObject *parent = nullptr);

public:

    static ExphoneConfig *instance();
    static QObject *qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
    {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)
        return instance();
    }

    QVariant value(const QString &key, const QVariant &def = QVariant()) const;
    void setValue(const QString &key, const QVariant &value);

    BOOL_OPTION(QStringLiteral("blockingCLIR"),    blockingCLIR,    setBlockingCLIR,    false)
    BOOL_OPTION(QStringLiteral("blockingUnknown"),    blockingUnknown,    setBlockingUnknown,    false)
private:
    using signal_ptr = void(ExphoneConfig::*)();

    void setValue(const QString &key, const QVariant &value, signal_ptr signal);

#ifdef MER_EDITION_SAILFISH
    MDConfGroup *m_group;
#endif
};

#undef ALARM_OPTION
#undef ENUM_OPTION
#undef STRING_OPTION
#undef INT64_OPTION
#undef UINT_OPTION
#undef INT_OPTION
#undef BOOL_OPTION
#undef OPTION

#endif // EXPHONECONFIG_H
