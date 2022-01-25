// SPDX-License-Identifier: Apache-2.0

#include <zephyr.h>

#include "qpc.h"
#include <stdlib.h>

int main()
{
    QF_init();

    printk("Hello QPc\n");

    return QF_run();
}

void QF_onStartup(void) {}
void QF_onCleanup(void) {}

void QF_onClockTick(void)
{
	QF_TICK_X(0U, (void *)0);
}
