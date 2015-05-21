/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company.  For licensing terms and
** conditions see http://www.qt.io/terms-conditions.  For further information
** use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file.  Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, The Qt Company gives you certain additional
** rights.  These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#ifndef NODEINSTANCEMETAOBJECT_H
#define NODEINSTANCEMETAOBJECT_H

#include <QQmlContext>
#include <QScopedPointer>
#include <private/qqmlopenmetaobject_p.h>
#include <private/qqmlvmemetaobject_p.h>

#include "nodeinstanceglobal.h"

namespace QmlDesigner {

namespace Internal {

class ObjectNodeInstance;
typedef QSharedPointer<ObjectNodeInstance> ObjectNodeInstancePointer;
typedef QWeakPointer<ObjectNodeInstance> ObjectNodeInstanceWeakPointer;

namespace QmlPrivateGate {

void createNewDynamicProperty(const ObjectNodeInstancePointer &nodeInstance, const QString &name);
void registerNodeInstanceMetaObject(const ObjectNodeInstancePointer &nodeInstance);

struct MetaPropertyData;

class MetaObject : public QQmlVMEMetaObject
{
public:
    ~MetaObject();

    static void registerNotifyPropertyChangeCallBack(void (*callback)(QObject*, const PropertyName &propertyName));

protected:
    MetaObject(const ObjectNodeInstancePointer &nodeInstance, QQmlEngine *engine);
    static MetaObject* getNodeInstanceMetaObject(const ObjectNodeInstancePointer &nodeInstance);

    void createNewDynamicProperty(const QString &name);
    int openMetaCall(QMetaObject::Call _c, int _id, void **_a);
    int metaCall(QMetaObject::Call _c, int _id, void **_a);
    void notifyPropertyChange(int id);
    void setValue(int id, const QVariant &value);
    QVariant propertyWriteValue(int, const QVariant &);

    QObject *myObject() const { return QQmlVMEMetaObject::object; }
    QAbstractDynamicMetaObject *parent() const { return const_cast<QAbstractDynamicMetaObject *>(dynamicMetaObjectParent()); }

    const QAbstractDynamicMetaObject *dynamicMetaObjectParent() const;

    const QMetaObject *metaObjectParent() const;

    int propertyOffset() const;

    int count() const;
    QByteArray name(int) const;

    void copyTypeMetaObject();

private:
    void init(QObject *, QQmlEngine *engine);

    ObjectNodeInstanceWeakPointer m_nodeInstance;
    QPointer<QQmlContext> m_context;
    QQmlOpenMetaObjectType *m_type;
    QScopedPointer<MetaPropertyData> m_data;
    //QAbstractDynamicMetaObject *m_parent;
    QQmlPropertyCache *m_cache;

    friend void createNewDynamicProperty(const ObjectNodeInstancePointer &nodeInstance, const QString &name);
    friend void registerNodeInstanceMetaObject(const ObjectNodeInstancePointer &nodeInstance);
};

} // namespace QmlPrivateGate
} // namespace Internal
} // namespace QmlDesigner

#endif // NODEINSTANCEMETAOBJECT_H
