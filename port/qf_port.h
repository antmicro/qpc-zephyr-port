// SPDX-License-Identifier: Apache-2.0

/**
* QF/C port to Zephyr OS, ARM Cortex-M
*
* Copyright (C) Antmicro Ltd
**/

#ifndef QF_PORT_H
#define QF_PORT_H

#define QF_EQUEUE_TYPE       QEQueue
#define QF_OS_OBJECT_TYPE    pthread_cond_t
#define QF_THREAD_TYPE       bool

#define QF_MAX_ACTIVE        64U
#define QF_MAX_TICK_RATE     2U
#define QF_ACTIVE_STOP        1

#define QF_EVENT_SIZ_SIZE    4U
#define QF_EQUEUE_CTR_SIZE   4U
#define QF_MPOOL_SIZ_SIZE    4U
#define QF_MPOOL_CTR_SIZE    4U
#define QF_TIMEEVT_CTR_SIZE  4U

#define QF_CRIT_ENTRY(dummy) QF_enterCriticalSection_()
#define QF_CRIT_EXIT(dummy)  QF_leaveCriticalSection_()

#include <posix/pthread.h>
#include "qep_port.h"
#include "qequeue.h"
#include "qmpool.h"
#include "qf.h"

void QF_enterCriticalSection_(void);
void QF_leaveCriticalSection_(void);
void QF_onClockTick(void);

#ifdef QP_IMPL

#define QF_SCHED_STAT_
#define QF_SCHED_LOCK_(dummy) ((void)0)
#define QF_SCHED_UNLOCK_()    ((void)0)

#define QACTIVE_EQUEUE_WAIT_(me_) \
    while ((me_)->eQueue.frontEvt == (QEvt *)0) \
        pthread_cond_wait(&(me_)->osObject, &QF_CriticalSectionMutex)
#define QACTIVE_EQUEUE_SIGNAL_(me_) \
    Q_ASSERT_ID(410, QF_active_[(me_)->prio] != (QActive *)0); \
        pthread_cond_signal(&(me_)->osObject)

#define QF_EPOOL_TYPE_            QMPool
#define QF_EPOOL_INIT_(p_, poolSto_, poolSize_, evtSize_) \
    (QMPool_init(&(p_), (poolSto_), (poolSize_), (evtSize_)))
#define QF_EPOOL_EVENT_SIZE_(p_)  ((uint_fast16_t)(p_).blockSize)
#define QF_EPOOL_GET_(p_, e_, m_, qs_id_) \
    ((e_) = (QEvt *)QMPool_get(&(p_), (m_), (qs_id_)))
#define QF_EPOOL_PUT_(p_, e_, qs_id_) \
    (QMPool_put(&(p_), (e_), (qs_id_)))

extern pthread_mutex_t QF_CriticalSectionMutex;

#endif /* QP_IMPL */
#endif /* QF_PORT_H */
