// SPDX-License-Identifier: Apache-2.0

#include <zephyr.h>

#include "qpc.h"
#include "safe_std.h"
#include <stdlib.h>

#define BLINKY_THREAD_STACK_SIZE		CONFIG_MAIN_STACK_SIZE

K_THREAD_STACK_DEFINE(blinky_stack, BLINKY_THREAD_STACK_SIZE);

Q_DEFINE_THIS_FILE

void BSP_ledOff(void) {
    PRINTF_S("%s\n", "LED OFF");
}
void BSP_ledOn(void) {
    PRINTF_S("%s\n", "LED ON");
}
void Q_onAssert(char const * const module, int loc) {
    FPRINTF_S(stderr, "Assertion failed in %s:%d", module, loc);
    exit(-1);
}

void QF_onStartup(void) {}
void QF_onCleanup(void) {}

void QF_onClockTick(void)
{
	QF_TICK_X(0U, (void *)0);  /* perform the QF clock tick processing */
}

enum BlinkySignals {
    TIMEOUT_SIG = Q_USER_SIG,
    MAX_SIG
};

typedef struct {
    QActive super;
    QTimeEvt timeEvt;
} Blinky;

static QState Blinky_initial(Blinky * const me, void const * const par);
static QState Blinky_off(Blinky * const me, QEvt const * const e);
static QState Blinky_on(Blinky * const me, QEvt const * const e);

static Blinky l_blinky;
QActive * const AO_Blinky = &l_blinky.super;

static void Blinky_ctor(void) {
    Blinky *me = (Blinky *)AO_Blinky;
    QActive_ctor(&me->super, Q_STATE_CAST(&Blinky_initial));
    QTimeEvt_ctorX(&me->timeEvt, &me->super, TIMEOUT_SIG, 0U);
}

int main()
{
    static QEvt const *blinky_queueSto[10];

    QF_init();

    printk("Hello QPc\n");

    Blinky_ctor();
    QACTIVE_START(AO_Blinky, 1U, blinky_queueSto, Q_DIM(blinky_queueSto),
                  (void *)blinky_stack, BLINKY_THREAD_STACK_SIZE,
                  (QEvt *)0);

    return QF_run();
}

static QState Blinky_initial(Blinky * const me, void const * const par)
{
    QTimeEvt_armX(&me->timeEvt, 100/2, 100/2);

    QS_FUN_DICTIONARY(&Blinky_off);
    QS_FUN_DICTIONARY(&Blinky_on);

    return Q_TRAN(&Blinky_off);
}

static QState Blinky_off(Blinky * const me, QEvt const * const e)
{
	QState status_;

	switch (e->sig) {
	case Q_ENTRY_SIG:
		BSP_ledOff();
		status_ = Q_HANDLED();
		break;
	case TIMEOUT_SIG:
		status_ = Q_TRAN(&Blinky_on);
		break;
	default:
		status_ = Q_SUPER(&QHsm_top);
		break;
	}

	return status_;
}

static QState Blinky_on(Blinky * const me, QEvt const * const e)
{
	QState status_;

	switch (e->sig) {
	case Q_ENTRY_SIG:
		BSP_ledOn();
		status_ = Q_HANDLED();
		break;
	case TIMEOUT_SIG:
		status_ = Q_TRAN(&Blinky_off);
		break;
	default:
		status_ = Q_SUPER(&QHsm_top);
		break;
	}

	return status_;
}
