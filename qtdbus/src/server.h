#pragma once

#include "common.h"

#include <QObject>

class Server : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", DBUS_INTERFACE);
    Q_PROPERTY(uint Count READ count NOTIFY CountChanged);

public:
    explicit Server(QObject *parent = nullptr);

    inline uint count() const { return m_count; }

Q_SIGNALS:
    void CountChanged(uint value);

public Q_SLOTS:
    uint IncrementBy(uint value);

private:
    void initDBus();

    uint m_count;
};
