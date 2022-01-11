// SPDX-License-Identifier: Apache-2.0

/**
 * QF/C port to Zephyr OS, ARM Cortex-M
 *
 * Copyright (C) Antmicro Ltd
 **/

#define QP_IMPL
#include "qf_port.h"
#include "qf_pkg.h"
#include "qassert.h"

#ifdef Q_SPY
#include "qs_port.h"
#include "qs_pkg.h"
#else
#include "qs_dummy.h"
#endif /* Q_SPY */

Q_DEFINE_THIS_MODULE("qf_port")

#define QF_DEFAULT_TICK_RATE_NSEC 10000000

pthread_mutex_t QF_CriticalSectionMutex;
static bool QF_running;
static struct timespec QF_tick;

void QF_init(void)
{
	QF_tick.tv_sec = 0;
	QF_tick.tv_nsec = QF_DEFAULT_TICK_RATE_NSEC;

	pthread_mutex_init(&QF_CriticalSectionMutex, NULL);
}

void QF_enterCriticalSection_(void)
{
	pthread_mutex_lock(&QF_CriticalSectionMutex);
}

void QF_leaveCriticalSection_(void)
{
	pthread_mutex_unlock(&QF_CriticalSectionMutex);
}

int_t QF_run(void)
{
	QF_onStartup();

	QF_running = true;

	while (QF_running) {
		QF_onClockTick();
		nanosleep(&QF_tick, NULL);
	}

	QF_onCleanup();
	pthread_mutex_destroy(&QF_CriticalSectionMutex);

	return 0;
}

void QF_stop(void)
{
    QF_running = false;
}

static void *task_func(void *arg)
{
	QActive *act = (QActive *)arg;

	act->thread = true;

	while (act->thread) {
		QEvt const *e = QActive_get_(act);
		QHSM_DISPATCH(&act->super, e, act->prio);
		QF_gc(e);
	}

	QF_remove_(act);

	return (void *)0;
}

void QActive_start_(QActive * const me, uint_fast8_t prio,
	QEvt const * * const qSto, uint_fast16_t const qLen,
	void * const stkSto, uint_fast16_t const stkSize,
	void const * const par)
{
	pthread_t thread;
	pthread_attr_t attr;

	QEQueue_init(&me->eQueue, qSto, qLen);
	pthread_cond_init(&me->osObject, NULL);

	me->prio = prio;
	QF_add_(me);
	QHSM_INIT(&me->super, par, me->prio);
	QS_FLUSH();

	pthread_attr_init(&attr);

	pthread_attr_setschedpolicy (&attr, SCHED_FIFO);
	pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED);
	pthread_attr_setstack(&attr, stkSto, stkSize);
	pthread_create(&thread, &attr, &task_func, me);

	pthread_attr_destroy(&attr);
}

void QActive_stop(QActive * const me)
{
	QActive_unsubscribeAll(me);
	me->thread = false;
}
