/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/
#ifndef QTHREADEDDOWNLOADMANAGER_P_H
#define QTHREADEDDOWNLOADMANAGER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qthreadeddownloadmanager.h"
#include <QThread>
#include <QMutex>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

QT_MODULE(Qt3D)

class QNetworkAccessManager;

//This is the main download thread where the network requests are queued and
//processed.
class QDownloadThread : public QThread
{
    Q_OBJECT
    friend class QDownloadInstance;
public:
    QDownloadThread(QObject *parent = 0);
    ~QDownloadThread();

    void run();

    bool isThreadQuitting() const;
protected:
    QNetworkAccessManager * m_netAccessMgr;
    int m_requestCount;
    bool m_threadQuitting;
};

//This is a single instance of a download request, and acts as an intermediary
//processing object between threads.
class QDownloadInstance : public QObject
{
    Q_OBJECT
public:
    explicit QDownloadInstance(QObject *parent = 0);
     ~QDownloadInstance();
    void doSetup(QThreadedDownloadManager &cComm);

signals:
    void downloadComplete(QByteArray assetData);
    void killDownloadThread();

public slots:
    void doWork(QUrl assetUrl);
    void doneWork();

protected:
    QNetworkAccessManager * getNetworkManager();
    void incrementRequestCount();
    void decrementRequestCount();
};

QT_END_NAMESPACE

QT_END_HEADER

#endif // QTHREADEDDOWNLOADMANAGER_P_H