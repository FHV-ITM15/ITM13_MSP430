#include <msp430.h> 

void main(void) {
	WDTCTL = WDTPW + WDTHOLD;	// Stop watchdog timer
	P1DIR |= BIT0; 				// Set P1.0 to output direction

	TA0CCR0 = 12000;			// Count limit (16 bit)
	TA0CCTL0 = 0x10;			// Enable counter interrupts, bit 4=1
	TA0CTL = TASSEL_1 + MC_1; 	// Timer A 0 with ACLK @ 12KHz, count UP
	_BIS_SR(LPM0_bits + GIE); 	// LPM0 (low power mode) with interrupts enabled
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0(void) {		// Timer0 A0 interrupt service routine

	P1OUT ^= BIT0;						// Toggle red LED
}


typedef struct {
	int id=0;
	int state=0;
	void (*threadFunc)(void);
	jmp_buf context;
} thread_t;
int lastId = 0;

int scheduler_startThread(void (*threadFunc)(void)) {
	ATOMIC_START();

	// new thread
	thread_t newThread;
	newThread.id = ++lastId;
	newThread.state = THREAD_READY;
	newThread.threadFunc = threadFunc;

	if (setjmp(gThreads[newThread].context) == 0) {
		// current context saved
		ATOMIC_END();
		return newThread;
	} else {
		// running here as active Thread
		void* SP = malloc(200);
		_set_SP_register(SP);

		ATOMIC_END();
		gThread[gRunningThread].tFunc();
		scheduler_killThread();
		return THREAD_ID_INVALID;
	}
}

void scheduler_runNextThread() {
	ATOMIC_START();
	threadID nextThread = determine
	next Thread(round robin)
	switch (gThreads[nextThread].state) {
	case THREAD_RUNNING:
		break; // already running
	case THREAD_READY:
		if (setjmp(gThreads[gRunningThread].context == 0) {
// saved current execution state
					if (gThreads[gRunningThread].state == THREAD_RUNNING)
					gThreads[gRunningThread].state = THREAD_READY;
					gRunningThread = nextThread;
					gThreads[gRunningThread].state = THREAD_RUNNING;
					longjmp(gThreads[gRunningThread].context, 1);
				}
				break;
				default:
				break; // no thread to run -> sleep no atomic
			}
			ATOMIC_END();
		}

		semaphor_P() {
			ATOMIC_START();
			dec counter;
			if (counter < 0) {
				queue < -current
				threadID
				scheduler_suspend(current threadID);
				scheduler_runNextThread();
			}
			ATOMIC_END();
		}
		semaphor_V() {
			ATOMIC_START();
			inc counter;
			if (counter <= 0) {
				scheduler_continueThread(queue_dequeue());
			}
			ATOMIC_END();
		}

