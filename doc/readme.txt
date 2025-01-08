/**  
\page Installation Installation
\image html rkh_bunner.jpg

Prev: \ref main_page "Home" \n
Next: \ref Porting "Porting"

The RKH project is organized through a hierarchical directory structure
to facilitate its distribution, support and maintenance.
This structure also includes application examples and demos on several
platforms. The following annotated directory tree lists the most important 
directories and files provided in the standard RKH distribution. The 
explanation section immediately following the directory tree explains the 
structure in more detail.

\anchor top_dir
\code
<RKH-root>					- RKH-root directory
|-- demo                    - Demostrative and example applications
|-- doc                     - Documentation
|-- source                  - Source and test code files of RKH
|-- template                - Templates for source code files
|-- third-party             - Third-party software components
|-- tools                   - Used tools
|-- copying.txt             - Licence file
\-- README                  - Change log file
\endcode
<STRONG> Figure 1 Top level directories </STRONG>

\n This section includes:

- \ref source_dir
- \ref portable_dir
- \ref demo_dir

\tableofcontents

<HR>
\section source_dir RKH source files

The following figure shows the upper level of \c /source directory. Every 
module has been structured to be testable and maintainable, using \c /inc, 
\c /src, and \c /test directories. Every implementation file of any module 
have its own test cases, which are included in corresponding directory within 
\c /test.

\anchor source_dir_fig
\code
<RKH-root>					        - RKH-root directory
|-- demo                            - Demostrative and example applications
|-- doc                             - Documentation
|-- source                          - Source and test code files of RKH
|   |-- fwk                         - Miscellaneous module
|   |-- queue                       - Fixed-size memory block module
|   |-- sm                          - State machine module
|   |-- sma                         - Active object module
|   |-- tmr                         - Software timer module
|   \-- trc                         - Runtime tracing module
|       |-- inc                     - Contains specification files of module
|       |   |-- rkhtrc_define.h     - Internal defines specification
|       |   |-- rkhtrc_filter.h     - Runtime trace filter specification
|       |   |-- rkhtrc.h            - Trace session specification
|       |   |-- rkhtrc_out.h        - Trace output specification
|       |   |-- rkhtrc_record.h     - Trace record specification
|       |   \-- rkhtrc_stream.h     - Trace stream specification
|       |-- src                     - Contains implementation files of module
|       |   |-- rkhtrc_filter.c     - Runtime trace filter
|       |   |-- rkhtrc_record.c     - Trace record
|       |   \-- rkhtrc_stream.c     - Trace stream 
|       \-- test                    - Contains unit test files of module
|           |-- build               -¨Project builder
|           |-- filter              - Unit test for rkhtrc_filter.c file
|           |   |-- inc             - Specification files of test cases
|           |   |-- runner          - Run the test cases
|           |   |-- src             - Implementation of test cases
|           |   \-- stub            - Test-doubles
|           |-- record              - Unit test for rkhtrc_record.c file
|           \-- stream              - Unit test for rkhtrc_stream.c file
|-- template                        - Templates for source code files
|-- third-party                     - Third-party software components
|-- tools                           - Used tools
|-- copying.txt                     - Licence file
\-- README                          - Change log file
\endcode
<STRONG> Figure 2 RKH source directory </STRONG>

<HR>
\section portable_dir RKH portable directory

All software components of the RKH, contain a platform abstraction 
layer. It is an indirection layer that hides the differences in 
hardware and software environments in which RKH operates, so that the RKH 
source code does not need to be changed to run in a different 
environment. Instead, all the changes required to adapt RKH are confined 
to this layer. It's basically composed by a few files, \b rkhplat.h and 
rkhtype.h, which include references to the actual platform-dependent 
files like \b rkhport.h and \b rkht.h. The platform-specific code for 
the RKH port is defined by \b rkhport.c. Not all RKH ports require this 
file.

As said above, each platform, compiler or processor supported by RKH must 
have its own platform-dependent file, called \b rkhport.h by RKH 
convention. 
Next, each \b rkhport.h file must be referenced from rkhplat.h header 
file, located in \c source/fwk/inc directory.  The next listing shows an 
example of \b rkhplat.h, where \c "__CFV1CW63__", and \c "__W32STVC08__" are 
used to instruct the C/C++ compiler to include header files from the specific 
RKH port directory, which must be created if not exists. The key point of the 
design is that all platform-independent RKH source files includes the same 
\b rkhplat.h header file as the application source files.

\code
#ifdef __CFV1CW63__
	#include "..\..\portable\cfv1\rkhs\cw6_3\rkhport.h"
#endif

#ifdef __W32STVC08__
#include "..\..\portable\80x86\win32_st\vc08\rkhport.h"
#endif
..
\endcode

The idea behind conditional compilation is that a \b rkhport.h can be 
electively compiled, depending upon whether a specific value has been 
efined.

\note The path of platform-dependent file must be relative.

The RKH uses a set of integer quantities. That maybe machine or compiler 
dependent. These types must be defined in \b rkht.h file.  
Each \b rkht.h file must be referenced from \b rkhtype.h header 
file, located in \c source/fwk/inc directory.  The next listing shows an 
example of \b rkhtype.h, where \c "__CFV1CW63__", and \c "__W32STVC08__" are 
used to instruct the C/C++ compiler to include header files from the specific 
RKH port directory. The key point of the design is that all 
platform-independent RKH source files include the same \b rkhtype.h 
header file as the application source files.

\code
#ifdef __CFV1CW63__
	#include "..\..\portable\cfv1\rkhs\cw6_3\rkht.h"
#endif

#ifdef __W32STVC08__
	#include "..\..\portable\80x86\win32_st\vc08\rkht.h"
#endif
...
\endcode

The idea behind conditional compilation is that a \b rkht.h can be 
selectively compiled, depending upon whether a specific value has been 
defined.

\note The path of platform-dependent file must be relative.

See the \ref Porting section for more information. 
The following figure shows the \c source/portable/ directory.

\anchor portable_dir_fig
\code
    <RKH-root>					        - RKH-root directory
    |-- demo                            - Demostrative and example applications
    |-- doc                             - Documentation
    |-- source                          - Source and test code files of RKH
    |   |-- fwk
    |   |-- mempool
( 1)|   |-- portable                    - Platform-specific RKH ports
( 2)|   |   |-- 80x86                   - Ports to the 80x86 processor
    |   |   |   |-- linux_st            - Ports to Linux with scheduler emulation
    |   |   |   |   \-- gnu             - Ports with the GNU compiler
    |   |   |   |       |-- rkhport.c   - RKH port to Linux source file
    |   |   |   |       |-- rkhport.h   - RKH platform-dependent include file
    |   |   |   |       \-- rkht.h      - RKH platform-dependent include file
    |   |   |   |-- win32_mt            - Ports to Win32 multithreding
    |   |   |   |   \-- vc              - Ports with the Visual Studio C++ IDE
( 3)|   |   |   \-- win32_st            - Ports to Win32 with scheduler emulation
( 4)|   |   |       \-- vc              - Ports with the Visual Studio C++ IDE
( 5)|   |   |           |-- rkhport.c
( 6)|   |   |           |-- rkhport.h
( 7)|   |   |           \-- rkht.h
    |   |   |-- arm-cortex              - Ports to the ARM Cortex processor
    |   |   |   |-- ksdk_os             - Ports to OS abstraction layer of KSDK 
    |   |   |   |   \-- ucosiii         - Ports to uCOS-III RTOS
    |   |   |   |       \-- kds         - Ports with KDS IDE
    |   |   |   \-- rkhs                - Ports to the native cooperative scheduler
    |   |   |       |-- arm_cm0         - Ports to the ARM Cortex-M0 processor
    |   |   |       |   \-- cw_v10      - Ports with the Codewarrior v10 compiler
    |   |   |       |-- arm_cm3         - Ports to the ARM Cortex-M3 processor
    |   |   |       |   \-- codered     - Ports with the code_red IDE
    |   |   |       |-- arm_cm4f        - Ports to the ARM Cortex-M4 processor
    |   |   |       |   \-- cw_v10      - Ports with the Codewarrior v10 compiler
    |   |   |       \-- ksdk
    |   |   |           \-- kds
( 8)|   |   |-- cfv1                    - Ports to the Coldfire V1 processor 
( 9)|   |   |   \-- rkhs                - Ports to the native cooperative scheduler
(10)|   |   |       \-- cw6_3           - Ports with the Codewarrior v6.3 compiler
(11)|   |   |           |-- rkhport.c
(12)|   |   |           |-- rkhport.h
(13)|   |   |           \-- rkht.h
    |   |   |-- s08                     - Ports to the S08 processor 
    |   |   |   \-- rkhs                - Ports to the native cooperative scheduler
    |   |   |       \-- cw6_3           - Ports with the Codewarrior v6.3 compiler
    |   |   |-- test                    - Only used for unit test
    |   |   \-- ucos                    - Ports to uCOS-III RTOS version 3
    |   |       \-- v3.03.01
    |   |-- queue
    |   |-- sm
    |   |-- sma
    |   |-- tmr
    |   \-- trc
    |-- template
    |-- third-party
    |-- tools
    |-- copying.txt
    \-- README
\endcode
<STRONG> Figure 3 RKH portable directory </STRONG>

\li ( 1) Platform-specific RKH ports. The directory \c source/portable/ contains 
platform-dependent 
files to be used by RKH applications. This directory structure is the most 
complicated because of the large number of choices available, such as 
CPU architectures, compilers, operating systems, and compiler options. 
Each of those choices is represented as a separate level of nesting in a 
hierarchical directory tree, so that each dimension in the multi-dimensional 
space of options can be extended independently from the others. Also, the 
directory branch for each port is individually customizable, so each branch 
can represent only choices relevant for a given CPU, operating system, 
compiler, etc.
\li ( 2) Ports to the 80x86 processor. The CPU architecture is placed as the 
first level of nesting within the \c source/portable/ directory. Examples of CPU 
architectures could be: 80x86, Coldfire, S08, ARM Cortex, ARM Thumb, 
MSP430, etc. 
Please note that a separate directory is needed whenever the CPU architecture 
is significantly different.
\li ( 3) Ports to Win32 with scheduler emulation. The second level of 
nesting, under the CPU architecture, is the operating system used.
\li ( 4) Ports with the Visual Studio C++ compiler. The next level of 
nesting, under each operating system directory, is the directory for the 
compiler used.
\li ( 5) RKH platform-dependent source file. The platform-specific source 
file is optional and many ports don't require it.
\li ( 6) RKH platform-dependent include file. Frequently, defines the 
interrupt locking method, the critical section management, among other things.
The key point of the design is that all platform-independent RKH source 
files include the same \c rkhplat.h header file as the application source 
files.
\li ( 7) RKH platform-dependent include file. In this file is defined the 
data types that uses RKH. The key point of the design is that all 
platform-independent RKH source files include the same \c rkhtype.h header 
file as the application source files.
\li ( 8) Ports to the Coldfire V1 processor
\li ( 9) using the native simple cooperative scheduler
\li (10) with the Codewarrior v6.3 compiler
\li (11-13) Idem (5-7)

<HR>
\section demo_dir RKH demo applications

The \c demo/ directory contains the application examples that are included in the 
standard RKH distribution. The structure of the \c demo/ is the most complicated 
because of the large number of choices available, such as CPU architectures, 
compilers, operating systems, and compiler options. Each of those choices is 
represented as a separate level of nesting in a hierarchical directory tree, 
so that each dimension in the multi-dimensional space of options can be 
extended independently from the others. Also, the directory branch for each 
RKH port is individually customizable, so each branch can represent only 
choices relevant for a given CPU, operating system, compiler, etc.

Each RKH application must have its own configuration file, called \c rkhcfg.h. 
This file adapts and configures RKH by means of compiler definitions and macros 
allowing to restrict the resources consumed by RKH. Adjusting this definitions 
allows to reduce the ROM and RAM consumption, and to enhance the system 
performance in a substantial manner.

\anchor demo_dir_fig
\code
    <RKH-root>					        - RKH-root directory
( 1)|-- demo                            - Demostrative and example applications
( 2)|   |-- 80x86                       - Demos for the x86 processor
( 5)|   |   |-- ahsm                    - Abstract State Machine example
( 7)|   |   |   |-- build               - IDE/Compiler dependent files
    |   |   |   |   |-- linux_st        - Linux with scheduler emulation
    |   |   |   |   |   \-- Makefile    - GNU toolchain project
( 3)|   |   |   |   \-- win32_st        - Win32 with scheduler emulation
( 4)|   |   |   |       \-- vc          - Visual Studio project
    |   |   |   |-- bsp.h               - Contains the BSP specification
    |   |   |   |-- main.c              - Contains the main() function along with an example for using the RKH trace facility
    |   |   |   |-- my.c                - Implements the 'my' state machine, which illustrates all aspects of implementing state machines with RKH framework
    |   |   |   |-- my.h                - Contains the declaration of active object, signals, and other facilities shared among the components of the application
    |   |   |   \-- rkhcfg.h            - Adapts and configures RKH
( 6)|   |   \-- subm                    - Submachine example
( 9)|   |-- cross                       - Cross-platform examples
    |   |   |-- blinky                  - Blinky example
    |   |   |   |-- build               - Platform (IDE/Compiler/OS/Processor) dependent files
    |   |   |   |   |-- 80x86           - For x86 processor
    |   |   |   |   |   |-- linux_st    - Linux with scheduler emulation
    |   |   |   |   |   \-- win32_st    - Win32 with scheduler emulation
    |   |   |   |   |-- arm-cortex                  - For ARM Cortex processors
    |   |   |   |   |   \-- rkhs                    - For the native scheduler
    |   |   |   |   |       |-- arm_cm0             - For ARM Cortex-M0 processor
    |   |   |   |   |       |   \-- frdm-kl25z      - For Freedom board frdm-kl25z
    |   |   |   |   |       |       \-- cw_v10      - For IDE Codewarrior v10
    |   |   |   |   |       |-- arm_cm3             - For ARM Cortex-M3 processor
    |   |   |   |   |       |   \-- lpcx-1769       - For specific lpcx-1769 processor
    |   |   |   |   |       |       \-- lpcxpresso  - For IDE Codewarrior v10
    |   |   |   |   |       \-- arm_cm4f            - For ARM Cortex-M4 processor
    |   |   |   |   |           |-- frdm-k64f       - For Freedom board frdm-k64f
    |   |   |   |   |           \-- twr_k60         - For Tower board K60
    |   |   |   |   |-- cfv1            - For Coldfire V1 processors
    |   |   |   |   \-- s08             - For S08 (8-bits) processors
    |   |   |   |-- blinky.c            - The Blinky active object implementation
    |   |   |   |-- blinky.h            - The Blinky active object specification
    |   |   |   |-- bsp.h               - Contains the BSP specification
    |   |   |   |-- main.c              - Contains the main() function along with an example for using the RKH trace facility
( 8)|   |   |   \-- rkhcfg.h            - Adapts and configures RKH
    |   |   \-- shared                  - Shared example
    |   |       |-- build               - Platform (IDE/Compiler/OS/Processor) dependent files
    |   |       |   |-- 80x86           - For x86 processor
    |   |       |   |   |-- win32_mt    - Win32 multithread
    |   |       |   |   \-- win32_st    - Win32 with scheduler emulation
    |   |       |   \-- arm-cortex                  - For ARM Cortex processors
    |   |       |       |-- ksdk_os                 - For OS abstraction layer of KSDK with OS
    |   |       |       |   \-- ucosiii             - For uCOS-III RTOS
    |   |       |       |       \-- frdm-k64f       - For Freedom board frdm-k64f
    |   |       |       |           \-- kds         - For IDE KDS
    |   |       |       \-- rkhs                    - For the native cooperative scheduler
    |   |       |           \-- ksdk_bm             - For OS abstraction layer of KSDK bare-metal
    |   |       |               \-- frdmk64f        - For Freedom board frdm-k64f
    |   |       |                   \-- kds         - For IDE KDS
    |   |       |-- bsp.h               - Contains the BSP specification
    |   |       |-- client.c            - The Client active object implementation
    |   |       |-- client.h            - The Client active object specification
    |   |       |-- main.c              - Contains the main() function along with an example for using the RKH trace facility
    |   |       |-- rkhcfg.h            - Adapts and configures RKH
    |   |       |-- server.c            - The Server active object implementation
    |   |       |-- server.h            - The Server active object specification
    |   |       \-- shared.h
    |   \-- libbsp                      - BSP library for several platforms
    |       |-- common
    |       \-- platform
    |           |-- 80x86
    |           |   |-- linux
    |           |   \-- win
    |           |-- arm-cortex
    |           |   |-- arm_cm0
    |           |   |   \-- frdm-kl25z
    |           |   |-- arm_cm3
    |           |   |   \-- lpc17xx
    |           |   \-- arm_cm4f
    |           |       |-- frdm-k64f
    |           |       \-- twr_k60
    |           |-- cfv1
    |           |   \-- DEMOQUE128
    |           \-- s08
    |               \-- DEMOQE128
    |-- doc
    |-- source
    |-- template
    |-- third-party
    |-- tools
    |-- copying.txt
    \-- README
\endcode
<STRONG> Figure 4 Demo application directories </STRONG>

\li ( 1) RKH demo applications. The \c demo/ directory contains the 
application examples that are included in the standard RKH distribution. 
The structure of the \c demo/ is the most complicated because of the large 
number of choices available, such as CPU architectures, compilers, operating 
systems, and compiler options. Each of those choices is represented as a 
separate level of nesting in a hierarchical directory tree, so that each 
dimension in the multi-dimensional space of options can be extended 
independently from the others. Also, the directory branch for each RKH port 
is individually customizable, so each branch can represent only choices 
relevant for a given CPU, operating system, compiler, etc.
\li ( 2) Demos for the x86 processor. The CPU architecture is placed as the 
first level of nesting within the \c demo/ directory. Examples of CPU 
architectures could be: 80x86, Coldfire, S08, ARM Cortex, ARM, MSP430, 
etc. Please note that a separate directory is needed whenever the CPU 
architecture is significantly different.
\li ( 3) Demos for Windows with scheduler emulation. The second level of 
nesting, under the CPU architecture, is the operating system used.
\li ( 4) Demos for Visual Studio C++. The next level of nesting, under each 
operating system directory, is the directory for the compiler used.
\li ( 5-6) Finally, the example application is located in its own directory. 
In this case, abstract hierarchical state machine example. This application 
is very useful to learn and explore the Statechart and UML features.
\li ( 7) The \c ahsm/build/ subdirectory contains the IDE-dependent files, 
like object files, executable, and among others.
\li ( 8) As mentioned above, each of application that use RKH must be defined 
its own configuration file, \c rkhcfg.h.
\li ( 9) This directory provides several examples on how to use RKH, 
designed to experiment with RKH immediately. The simplicity of the code and 
state diagrams means it also serves as a good starter project for people who 
are not yet familiar with RKH. Also, this examples are demostration projects 
considered cross-platform, because of they are able to function on more than 
one computer architecture or operating system. The code of demos are located 
in the \c \<rkh\>/demo/cross/ directory, where \c \<rkh\> stands for the 
installation directory chosed to install the accompanying software. Each 
example contains the platform-independent source code and a  
.. \c build/ directory. The structure of the .. \c build/ branch 
closely mirrors the structure of the \c source/portable/ directory, which 
contains platform-specific header files and libraries to be used by RKH 
applications.

Prev: \ref main_page "Home" \n
Next: \ref Porting "Porting"

\page Porting Porting
\image html rkh_bunner.jpg

Prev: \ref main_page "Home" \n
Next: \ref cfg "Configuration"

This section describes how to adapt the RKH to various platforms, which
is a process called porting. RKH contains a clearly defined abstraction
layer, which encapsulates all the platform-specific code and cleanly
separates it from platform-neutral code.

Porting RKH implies to create the a few platform-dependent files, 
\c rhhport.h, \c rkhport.c, \c rkht.h, which frequently defines the interrupt 
locking method, the critical section management, data types, among other things.

\note
It is recommended to start from a similar and existing port, properly copying 
and modifying its files (\c rkhport.c, \c rkhport.h and \c rkht.h) according 
to your needs and platform in question.

The RKH directories and files are described in detail in \ref Installation 
section. The next sections listed below describes the aspects to be considered 
to port RKH.

-# \ref files
-# \ref data
-# \ref rom
-# \ref blk 
-# \ref dyn
-# \ref hk
-# \ref ilock
-# \ref crt
-# \ref trc
-# \ref rkhp
-# \ref extra

\tableofcontents

<HR>
\section files Platform-dependent files

Create a directory into \c source/fwk/portable within its platform 
dependent-files (\c rkhport.c, \c rkhport.h and \c rkht.h).
Please, see \ref portable_dir section.

<HR>
\section data Data types definitions

Create the data type file. The RKH uses a set of integer quantities. 
That maybe machine or compiler dependent. These types must be defined in 
\c rkht.h file. The following listing shows the required data type definitions:

\include types.h

Please, see \ref portable_dir section.

<HR>
\section rom ROM allocator

For declaring an object that its value will not be changed and that
will be stored in ROM, must be defined in \c rkhport.h the \c RKHROM macro.
Example:
\code
#define RKHROM			const
\endcode

<HR>
\section blk Active Object execution

RKH works in conjunction with a traditional OS/RTOS?
[ \ref q1y "YES" ] - [ \ref q1n "NO" ]
<HR>

\anchor q1y
<I>RKH works in conjunction with a traditional OS/RTOS?: \b YES </I>\n
Is a multithreaded application?
[ \ref q2y "YES" ] - [ \ref q2n "NO" ]

<HR>

\anchor q2y
<I>Is a multithreaded application?: \b YES </I>\n
The RKH framework can work with virtually any traditional OS/RTOS. 
Combined with a conventional RTOS, RKH takes full advantage of the 
multitasking capabilities of the RTOS by executing each active object (SMA) 
in a separate task or thread.

-# Define the macros 
   #RKH_CFGPORT_NATIVE_SCHEDULER_EN = #RKH_DISABLED,
   #RKH_CFGPORT_SMA_THREAD_EN = #RKH_ENABLED,
   #RKH_CFGPORT_REENTRANT_EN = #RKH_ENABLED, within the \c rkhport.h file.
-# Define the macros 
   #RKH_THREAD_TYPE, 
   #RKH_CFGPORT_SMA_STK_EN and 
   #RKH_THREAD_STK_TYPE in \c rkhport.h according to underlying OS/RTOS. 
-# Define the macro #RKH_CFGPORT_SMA_STK_EN in \c rkhport.h according to 
   OS/RTOS.
-# Then, implement the platform-specific functions rkh_fwk_init(), 
   rkh_fwk_enter(), rkh_fwk_exit(), rkh_sma_activate(), and 
   rkh_sma_terminate() in \c rkhport.c, which are specified in 
   \c source/fwk/inc/rkhfwk_sched.h and \c source/sma/inc/rkhsma.h
   respectively.

<EM>Example for ARM Cortex-M4 Kinetis, KDS, KSDK and uC/OS-III</EM> \n
Fragment of \c rkhport.h
See \c source/portable/arm-cortex/ksdk_os/ucosiii/kds/rkhport.h
\code
#define RKH_CFGPORT_NATIVE_SCHEDULER_EN     RKH_DISABLED
#define RKH_CFGPORT_SMA_THREAD_EN           RKH_ENABLED
#define RKH_CFGPORT_REENTRANT_EN            RKH_ENABLED
#define RKH_THREAD_TYPE             	    OS_TCB
#define RKH_THREAD_STK_TYPE                 rui8_t
\endcode

Fragment of \c rkhport.c. 
See \c source/portable/arm-cortex/ksdk_os/ucosiii/kds/rkhport.c
\code
void 
rkh_fwk_init(void)
{
    osa_status_t status;

    tick_cnt = RKH_TICKS_RATE;
    status = OSA_Init();

    OS_AppTimeTickHookPtr = rkh_isr_tick;
    OS_AppIdleTaskHookPtr = idle_thread_function;

    RKH_ENSURE(status == kStatus_OSA_Success);
}

void 
rkh_fwk_enter(void)
{
    osa_status_t status;
    RKH_SR_ALLOC();

    RKH_HOOK_START();                               /* RKH start-up callback */
    RKH_TR_FWK_EN();
    RKH_TR_FWK_OBJ(&rkh_isr_tick);

    status = OSA_Start();                /* uC/OS-III start the multitasking */
    RKH_TRC_CLOSE();                            /* cleanup the trace session */
    RKH_ENSURE(status == kStatus_OSA_Success);     /* NEVER supposed to come */
                                                       /* back to this point */
}

void 
rkh_fwk_exit(void)
{
    RKH_SR_ALLOC();
    RKH_TR_FWK_EX();
    RKH_HOOK_EXIT();                                 /* RKH cleanup callback */
}

void 
rkh_sma_activate(RKH_SMA_T *sma, const RKH_EVT_T **qs, RKH_QUENE_T qsize, 
				 void *stks, rui32_t stksize)
{
    msg_queue_handler_t qh;
    osa_status_t status;
    task_handler_t th;

    RKH_REQUIRE(qs != (const RKH_EVT_T **)0);

    sma->equeue.msgs = (void *)qs;
    qh = OSA_MsgQCreate(&sma->equeue,          /* event message queue object */
                        (uint16_t)qsize,       /* max. size of message queue */
                        /* allocate pointers */
                        (uint16_t)(sizeof(RKH_EVT_T *) / sizeof(uint32_t)));
    RKH_ENSURE(qh != (msg_queue_handler_t)0);

    rkh_sma_register(sma);
    rkh_sm_init((RKH_SM_T *)sma);

    th = &sma->thread;
    status = OSA_TaskCreate(thread_function,                     /* function */
                            "uc_task",                               /* name */
                            (uint16_t)stksize,                 /* stack size */
                            (task_stack_t *)stks, /* stack base (low memory) */
                            (uint16_t)RKH_GET_PRIO(sma),         /* priority */
                            sma,                        /* function argument */
                            false,                /* usage of float register */
                            &th);    /* task object */

    RKH_ENSURE(status == kStatus_OSA_Success);
    sma->running = (rbool_t)1;
}

void 
rkh_sma_terminate(RKH_SMA_T *sma)
{
    osa_status_t status;

    sma->running = (rbool_t)0;
    status = OSA_MsgQDestroy(&sma->equeue);
    RKH_ENSURE(status == kStatus_OSA_Success);
}
\endcode
[ \ref q3 "NEXT" ]
<HR>

\anchor q2n
<I>Is a multithreaded application?: \b NO </I>\n
It's frequently used for single thread applications that uses its own 
cooperative and non-preemptive scheduler, where RKH provides its own priority 
mechanism.

-# Define the macros 
   #RKH_CFGPORT_NATIVE_SCHEDULER_EN = #RKH_DISABLED,
   #RKH_CFGPORT_SMA_THREAD_EN = #RKH_DISABLED,
   #RKH_CFGPORT_REENTRANT_EN = #RKH_DISABLED, 
   #RKH_CFGPORT_SMA_STK_EN = #RKH_DISABLED, 
   within the \c rkhport.h file.
-# Include the \c source/sma/inc/rkhsma_prio.h in \c rkhport.c. 
-# Implement the functions rkh_sma_block(), rkh_sma_setReady(), and 
   rkh_sma_setUnready(), which are specified in 
   \c source/sma/inc/rkhsma_sync.h, using the functions provided by 
   \c source/sma/inc/rkhsma_prio.h

<EM>Example for Windows single-thread scheduling</EM> \n
Fragment of \c rkhport.c. 
See \c source/portable/80x86/win32_st/vc/rkhport.c
\code
void
rkh_sma_block(RKH_SMA_T *const me)
{
    RKH_ASSERT(me->equeue.qty != 0);
}

void
rkh_sma_setReady(RKH_SMA_T *const me)
{
    rkh_smaPrio_setReady(RKH_SMA_ACCESS_CONST(me, prio));
    SetEvent(sma_is_rdy);
}

void
rkh_sma_setUnready(RKH_SMA_T *const me)
{
    rkh_smaPrio_setUnready(RKH_SMA_ACCESS_CONST(me, prio));
}
\endcode
[ \ref q3 "NEXT" ]
<HR>

\anchor q3
Are implemented the event queues with a message queue of the underlying OS/RTOS?
[ \ref q3y "YES" ] - [ \ref q3n "NO" ]
<HR>

\anchor q3y
<I>Are implemented the event queues with a message queue of the underlying 
OS/RTOS?: \b YES </I>\n
-# Define the macro #RKH_CFGPORT_NATIVE_EQUEUE_EN = #RKH_DISABLED in 
   \c rkhport.h
-# Define the macro #RKH_EQ_TYPE in \c rkhport.h according to OS/RTOS.
-# Then, implement the platform-specific functions rkh_sma_post_fifo(), 
   rkh_sma_post_lifo() and rkh_sma_get(), which are specified in 
   \c source/sma/inc/rkhsma.h. All these functions are frequently placed in
   \c rkhport.c file.
-# Define #RKH_CFGPORT_SMA_THREAD_DATA_EN = #RKH_DISABLED 
-# Define the macro #RKH_CFGPORT_SMA_QSTO_EN in \c rkhport.h according to 
   OS/RTOS.

<EM>Example for ARM Cortex-M4 Kinetis, KDS, KSDK and uC/OS-III</EM> \n
Fragment of \c rkhport.h
See \c source/portable/arm-cortex/ksdk_os/ucosiii/kds/rkhport.h
\code
#define RKH_EQ_TYPE                         msg_queue_t
\endcode

<EM>Example for ARM Cortex-M4 Kinetis, KDS, KSDK and uC/OS-III</EM> \n
Fragment of \c rkhport.c
See \c source/portable/arm-cortex/ksdk_os/ucosiii/kds/rkhport.c
\code
void 
rkh_sma_post_fifo(RKH_SMA_T *sma, const RKH_EVT_T *e)
{
    osa_status_t status;
    RKH_SR_ALLOC();

    RKH_HOOK_SIGNAL(e);
    RKH_ENTER_CRITICAL_();

    RKH_INC_REF(e);
    status = OSA_MsgQPut(&sma->equeue,                 /* event queue object */
                         (void *)&e);                 /* actual event posted */
    RKH_ALLEGE(status == kStatus_OSA_Success);
    RKH_TR_SMA_FIFO(sma, e, sender, e->pool, e->nref, 
                        &sma->equeue.queue.MsgQ.NbrEntries, 0);

    RKH_EXIT_CRITICAL_();
}

void 
rkh_sma_post_lifo( RKH_SMA_T *sma, const RKH_EVT_T *e )
{
    osa_status_t status;
    RKH_SR_ALLOC();

    RKH_HOOK_SIGNAL(e);
    RKH_ENTER_CRITICAL_();

    RKH_INC_REF(e);
    status = OSA_MsgQPutLifo(&sma->equeue,             /* event queue object */
                             (void *)&e);             /* actual event posted */
    RKH_ALLEGE(status == kStatus_OSA_Success);
    RKH_TR_SMA_LIFO(sma, e, sender, e->pool, e->nref, 
                        &sma->equeue.queue.MsgQ.NbrEntries, 0);

    RKH_EXIT_CRITICAL_();
}

RKH_EVT_T *
rkh_sma_get(RKH_SMA_T *sma)
{
    osa_status_t status;
    RKH_EVT_T *e;
    RKH_SR_ALLOC();

    status = OSA_MsgQGet(&sma->equeue,                 /* event queue object */
                         &e,                       /* received event message */
                         OSA_WAIT_FOREVER);             /* wait indefinitely */
    RKH_ENSURE(status == kStatus_OSA_Success);
    /* To get the number of message currently in the queue and its */
    /* minimum number of free elements is not a reliable manner in OSA */
    /* Because the variables are obtained outside critical section could be */
    /* a race condition */
    /* Morever, OSA abstraction layer not implements the water mark in */
    /* the queue */
    RKH_TR_SMA_GET(sma, e, e->pool, e->nref,
                        &sma->equeue.queue.MsgQ.NbrEntries, 0);
    return e;
}
\endcode
[ \ref dyn "NEXT" ]
<HR>

\anchor q3n
<I>Are implemented the event queues with a message queue of the underlying 
OS/RTOS?: \b NO </I>\n
-# Define the macro #RKH_CFGPORT_NATIVE_EQUEUE_EN = #RKH_ENABLED in 
   \c rkhport.h
-# Define the macro #RKH_CFG_QUE_EN = #RKH_ENABLED in \c rkhcfg.h
-# Implement the platform-specific functions rkh_sma_block(), 
   rkh_sma_setReady(), and rkh_sma_setUnready() in \c rkhport.c according to 
   underlying OS/RTOS, which are specified in \c source/sma/inc/rkhsma_sync.h 
   file.
-# Define #RKH_CFGPORT_SMA_THREAD_DATA_EN = #RKH_ENABLED and #RKH_OSSIGNAL_TYPE
   according to underlying OS/RTOS
-# Define the macro #RKH_CFGPORT_SMA_QSTO_EN = #RKH_ENABLED in \c rkhport.h

<EM>Example for Windows multithreaded application</EM> \n
Fragment of \c rkhport.h. 
See \c source/portable/80x86/win32_mt/vc/rkhport.h
\code
#define RKH_OSSIGNAL_TYPE                   void*
\endcode

Fragment of \c rkhport.c. 
See \c source/portable/80x86/win32_mt/vc/rkhport.c
\code
void
rkh_sma_block(RKH_SMA_T *const me)
{
    while (me->equeue.qty == 0)
    {
        rkhport_exit_critical();
        (void)WaitForSingleObject(me->os_signal, (DWORD)INFINITE);
        rkhport_enter_critical();
    }
}

void
rkh_sma_setReady(RKH_SMA_T *const me)
{
    (void)SetEvent(me->os_signal);
}

void
rkh_sma_setUnready(RKH_SMA_T *const me)
{
    (void)me;
}
\endcode
[ \ref dyn "NEXT" ]
<HR>

\anchor q1n
<I>RKH works in conjunction with a traditional OS/RTOS?: \b NO </I>\n
-#  Define the macros #RKH_CFGPORT_NATIVE_SCHEDULER_EN = #RKH_ENABLED,
    #RKH_CFGPORT_SMA_THREAD_EN = #RKH_DISABLED, 
    #RKH_CFGPORT_SMA_THREAD_DATA_EN = #RKH_DISABLED, 
    #RKH_CFGPORT_SMA_STK_EN = #RKH_DISABLED, 
    #RKH_CFGPORT_REENTRANT_EN = #RKH_DISABLED in the \c rkhport.h file.
-#  Define the macro #RKH_EQ_TYPE = RKH_QUEUE_T in \c rkhport.h

[ \ref q4 "NEXT" ]
<HR>

\anchor q4
Are implemented the event queues with the native queues?
[ \ref q4y "YES" ] - [ \ref q4n "NO" ]
<HR>

\anchor q4y
<I>Are implemented the event queues with the native queues?: \b YES </I>\n
-# Define the macro #RKH_CFGPORT_NATIVE_EQUEUE_EN = #RKH_ENABLED and 
   #RKH_CFG_QUE_EN = #RKH_ENABLED in \c rkhport.h and \c rkhcfg.h respectively.
-# Define the macro #RKH_CFGPORT_SMA_QSTO_EN = #RKH_ENABLED in \c rkhport.h

[ \ref dyn "NEXT" ]
<HR>

\anchor q4n
<I>Are implemented the event queues with the native queues?: \b NO </I>\n
-# Define the macro #RKH_CFGPORT_NATIVE_EQUEUE_EN = #RKH_DISABLED in 
   \c rkhport.h
-# Define the macro #RKH_EQ_TYPE in \c rkhport.h according to external queue 
   module.
-# Then, implement the platform-specific functions rkh_sma_post_fifo(), 
   rkh_sma_post_lifo() and rkh_sma_get(), which are specified in 
   \c source/sma/inc/rkhsma.h. All these functions are frequently placed in
   \c rkhport.c file. See section YES of \ref q3.
-# Define the macro #RKH_CFGPORT_SMA_QSTO_EN in \c rkhport.h according to 
   external queue module..

[ \ref dyn "NEXT" ]
<HR>
\section dyn Dynamic event support

Is required events with arguments?
[ \ref dynq1y "YES" ] - [ \ref dynq1n "NO" ]
<HR>

\anchor dynq1y
<I>Is required events with arguments?: \b YES </I>\n
If RKH works in conjunction with a traditional OS/RTOS, is implemented 
the dynamic memory support with a internal module of the underlying 
OS/RTOS?
[ \ref dynq2y "YES" ] - [ \ref dynq2n "NO" ]
<HR>

\anchor dynq2y
<I>If RKH works in conjunction with a traditional OS/RTOS, is implemented 
the dynamic memory support with a internal module of the underlying 
OS/RTOS?: \b YES </I>\n
-# Define the macro #RKH_CFG_FWK_DYN_EVT_EN = #RKH_ENABLED in \c rkhcfg.h
-# Define the macro #RKH_CFGPORT_NATIVE_DYN_EVT_EN = #RKH_DISABLED in 
   \c rkhport.h
-# Then, implement the platform-specific functions rkh_evtPool_init(), 
   rkh_evtPool_getPool(), rkh_evtPool_getBlockSize(), rkh_evtPool_get(), 
   rkh_evtPool_put(), rkh_evtPool_getNumUsed(), rkh_evtPool_getNumMin() and 
   rkh_evtPool_getNumBlock(), which are specified in 
   \c source/fwk/inc/rkhfwk_evtpool.h. All these functions are frequently 
   placed in \c rkhport.c file.

<EM>Example for memory partitions of uC/OS-III</EM> \n
\code
/* ---------------------------- Local data types --------------------------- */
struct RKHEvtPool
{
    OS_MEM memPool;
    rui8_t used;
};

/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static RKHEvtPool evtPools[RKH_CFG_FWK_MAX_EVT_POOL];

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
void 
rkh_evtPool_init(void)
{
    rInt i;
    RKHEvtPool *ep;

    for (i = 0, ep = evtPools; i < RKH_CFG_FWK_MAX_EVT_POOL; ++i, ++ep)
    {
        ep->used = 0;
    }
}

RKHEvtPool *
rkh_evtPool_getPool(void *stoStart, rui16_t stoSize, RKH_ES_T evtSize)
{
    rInt i;
    RKHEvtPool *ep;
    OS_ERR err;

    for (i = 0, ep = evtPools; i < RKH_CFG_FWK_MAX_EVT_POOL; ++i, ++ep)
    {
        if (ep->used == 0)
        {
            OSMemCreate((OS_MEM *)ep,
                        (CPU_CHAR *)"ep",
                        stoStart,
                        (OS_MEM_QTY )(stoSize / evtSize),
                        (OS_MEM_SIZE)evtSize,
                        &err);
            /* Check 'err' */
            return ep;
        }
    }
    return (RKHEvtPool *)0;
}

rui8_t 
rkh_evtPool_getBlockSize(RKHEvtPool *const me)
{
    RKH_REQUIRE(me != (RKHEvtPool *)0);
    return (rui8_t)((OS_MEM *)me)->BlkSize;
}

RKH_EVT_T *
rkh_evtPool_get(RKHEvtPool *const me)
{
    OS_ERR err;

    RKH_REQUIRE(me != (RKHEvtPool *)0);
    return (RKH_EVT_T *)OSMemGet((OS_MEM *)me, &err);
}

void 
rkh_evtPool_put(RKHEvtPool *const me, RKH_EVT_T *evt)
{
    OS_ERR err;

    RKH_REQUIRE(me != (RKHEvtPool *)0);
    rkh_memPool_put((OS_MEM *)me, (void *)evt, &err);
}

rui8_t 
rkh_evtPool_getNumUsed(RKHEvtPool *const me)
{
    RKH_REQUIRE(me != (RKHEvtPool *)0);
    return (rui8_t)(((OS_MEM *)me)->NbrMax - ((OS_MEM *)me)->NbrFree);
}

rui8_t 
rkh_evtPool_getNumMin(RKHEvtPool *const me)
{
    RKH_REQUIRE(me != (RKHEvtPool *)0);
    return (rui8_t)0;   /* This operation is not supported for Micrium */
}

rui8_t 
rkh_evtPool_getNumBlock(RKHEvtPool *const me)
{
    RKH_REQUIRE(me != (RKHEvtPool *)0);
    return (rui8_t)((OS_MEM *)me)->NbrMax;
}
\endcode
[ \ref hk "NEXT" ]
<HR>

\anchor dynq1n
<I>Is required events with arguments?: \b NO </I>\n
-# Define the macro #RKH_CFG_FWK_DYN_EVT_EN = #RKH_DISABLED in \c rkhcfg.h
-# Define the macro #RKH_CFGPORT_NATIVE_DYN_EVT_EN = #RKH_DISABLED in 
   \c rkhport.h

[ \ref dynq3 "NEXT" ]
<HR>

\anchor dynq2n
<I>If RKH works in conjunction with a traditional OS/RTOS, is implemented 
the dynamic memory support with a internal module of the underlying 
OS/RTOS?: \b NO </I>\n
-# Define the macro #RKH_CFG_FWK_DYN_EVT_EN = #RKH_ENABLED and 
   #RKH_CFG_MP_EN = #RKH_ENABLED in \c rkhcfg.h
-# Define the macro #RKH_CFGPORT_NATIVE_DYN_EVT_EN = #RKH_ENABLED in 
   \c rkhport.h

[ \ref dynq3 "NEXT" ]
<HR>

\anchor dynq3
If the application code uses dynamic memory allocation, it's implemented 
with the native fixed-size memory block pool?
[ \ref dynq3y "YES" ] - [ \ref dynq3n "NO" ]
<HR>

\anchor dynq3y
<I>If the application code uses dynamic memory allocation, it's implemented 
with the native fixed-size memory block pool?: \b YES </I>\n
-# Define the macro #RKH_CFG_FWK_DYN_EVT_EN = #RKH_ENABLED and 
   #RKH_CFG_MP_EN = #RKH_ENABLED in \c rkhcfg.h
-# Define the macro #RKH_CFGPORT_NATIVE_DYN_EVT_EN = #RKH_ENABLED in 
   \c rkhport.h

[ \ref hk "NEXT" ]
<HR>

\anchor dynq3n
<I>If the application code uses dynamic memory allocation, it's implemented 
with the native fixed-size memory block pool?: \b NO </I>\n
-# Define the macro #RKH_CFG_FWK_DYN_EVT_EN = #RKH_ENABLED in \c rkhcfg.h
-# Define the macro #RKH_CFGPORT_NATIVE_DYN_EVT_EN = #RKH_DISABLED in 
   \c rkhport.h
-# Then, implement the platform-specific functions rkh_evtPool_init(), 
   rkh_evtPool_getPool() and rkh_evtPool_getBlockSize(), rkh_evtPool_get(), 
   rkh_evtPool_put(), rkh_evtPool_getNumUsed(), rkh_evtPool_getNumMin() and 
   rkh_evtPool_getNumBlock() which are specified in 
   \c source/fwk/inc/rkhfwk_evtpool.h. All these functions are frequently 
   placed in \c rkhport.c file.

[ \ref hk "NEXT" ]

<HR>
\section hk Hook functions
A RKH port cannot and should not define all the functions that it calls, 
because this would render the port too inflexible. The functions that RKH 
calls but doesn't actually implement are referred to as callback or hook 
functions. All these functions in RKH are easily indentifiable by the 
\b "_hook_" key word used in the function name, rkh_hook_dispatch(), 
rkh_hook_signal(), rkh_hook_timeout(), rkh_hook_start(), rkh_hook_exit(), 
and rkh_hook_idle(). 
Please, see #RKH_CFG_HOOK_DISPATCH_EN, #RKH_CFG_HOOK_SIGNAL_EN, #RKH_CFG_HOOK_TIMEOUT_EN, #RKH_CFG_HOOK_START_EN, #RKH_CFG_HOOK_EXIT_EN, #RKH_CFG_HOOK_TIMETICK_EN, and #RKH_CFG_HOOK_PUT_TRCEVT_EN options from the \c rkhcfg.h.\n

\code void rkh_hook_dispatch(RKH_SMA_T *sma, RKH_EVT_T *e)\endcode
\copydetails RKH_CFG_HOOK_DISPATCH_EN

\code void rkh_hook_signal(RKH_EVT_T *e)\endcode
\copydetails RKH_CFG_HOOK_SIGNAL_EN

\code void rkh_hook_timeout(const void *t)\endcode
\copydetails RKH_CFG_HOOK_TIMEOUT_EN

\code void rkh_hook_start(void)\endcode
\copydetails RKH_CFG_HOOK_START_EN

\code void rkh_hook_exit(void)\endcode
\copydetails RKH_CFG_HOOK_EXIT_EN

\code void rkh_hook_idle(void)\endcode
\copydetails rkh_hook_idle

\code void rkh_hook_timetick(void)\endcode
\copydetails RKH_CFG_HOOK_TIMETICK_EN

<HR>
\section ilock Interrupt locking mechanism

\copybrief RKH_ENA_INTERRUPT()
\copydetails RKH_ENA_INTERRUPT()

<HR>
\section crt Critical section

\copybrief RKH_CPUSR_TYPE()
\copydetails RKH_CPUSR_TYPE()

<HR>
\section trc Trace facility

When using the trace facility must be defined in \c rkhport.h the following 
configurations:

-# #RKH_CFGPORT_TRC_SIZEOF_PTR \n
   \copybrief #RKH_CFGPORT_TRC_SIZEOF_PTR
   \copydetails #RKH_CFGPORT_TRC_SIZEOF_PTR  
-# #RKH_CFGPORT_TRC_SIZEOF_PTR \n
   \copybrief #RKH_CFGPORT_TRC_SIZEOF_FUN_PTR 
   \copydetails #RKH_CFGPORT_TRC_SIZEOF_FUN_PTR 
-# #RKH_CFGPORT_TRC_SIZEOF_TSTAMP \n 
   \copybrief #RKH_CFGPORT_TRC_SIZEOF_TSTAMP 
   \copydetails #RKH_CFGPORT_TRC_SIZEOF_TSTAMP

\n
<EM>Example for ARM Cortex</EM>\n
\code 
#define RKH_CFGPORT_TRC_SIZEOF_PTR			32u
#define RKH_CFGPORT_TRC_SIZEOF_FUN_PTR		32u
#define RKH_CFGPORT_TRC_SIZEOF_TSTAMP		32u
\endcode

<EM>Example for S08 of NXP</EM> \n
\code 
#define RKH_CFGPORT_TRC_SIZEOF_PTR			16u
#define RKH_CFGPORT_TRC_SIZEOF_FUN_PTR		16u
#define RKH_CFGPORT_TRC_SIZEOF_TSTAMP		16u
\endcode

A RKH port cannot and should not define all the functions that it calls, 
because this would render the port too inflexible. Therefore, the 
application-specific functions \c rkh_trc_open(), \c rkh_trc_close(), 
\c rkh_trc_flush(), and \c rkh_trc_getts() are application provided, which 
are specified in \c source/trc/inc/rkhtrc_out.h, typically in the board 
support package (\c bsp.c).
\n
\code void rkh_trc_open(void)\endcode
\copydetails RKH_TRC_OPEN \n

\code void rkh_trc_close(void)\endcode
\copydetails RKH_TRC_CLOSE \n

\code void rkh_trc_flush(void)\endcode
\copydetails RKH_TRC_FLUSH \n

\code void rkh_trc_getts(void)\endcode
\copydetails rkh_trc_getts \n

<HR>
\section rkhp A port file example

<EM>\b rkhport.h for ARM Cortex-M3 and LPCXpresso, with native scheduler</EM> \n
See \c source/portable/arm-cortex/rkhs/arm_cm3/codered/rkhport.h file

\include rkhport.h

<EM>\b rkht.h for ARM Cortex-M3 and LPCXpresso</EM> \n
See \c source/portable/arm-cortex/rkhs/arm_cm3/codered/rkht.h file

\include rkht.h

<EM>\b rkhport.c for ARM Cortex-M3 and LPCXpresso</EM> \n
See \c source/portable/arm-cortex/rkhs/arm_cm3/codered/rkhport.c file

\include rkhport.c

<HR>
\section extra Extras

<EM>In \c rkhport.h</EM> \n
See \c source/portable/arm-cortex/rkhs/arm_cm3/codered/rkhport.h file
\code
const char *rkhport_get_version(void);
const char *rkhport_get_desc(void);
\endcode

<EM>In \c rkhport.c</EM> \n
See \c source/portable/arm-cortex/rkhs/arm_cm3/codered/rkhport.c file
\code
RKH_MODULE_NAME(rkhport)
RKH_MODULE_VERSION(rkhport, 1.00)
RKH_MODULE_DESC(rkhport, "ARM Cortex-M, LPCXpresso")

const char *
rkhport_get_version(void)
{
    return RKH_MODULE_GET_VERSION();
}

const char *
rkhport_get_desc(void)
{
    return RKH_MODULE_GET_DESC();
}
\endcode

\page qref Quick reference
\image html rkh_bunner.jpg

Prev: \ref main_page "Home" \n
Next: \ref Installation "Installation"

The main objective of this section is to show and also illustrate the most 
important concepts to apply when dealing with RKH and the event-driven 
applications.

This section includes:

- \ref qref0
- \ref qref1
- \ref qref2
- \ref qrefsb
- \ref qref16
- \ref qref3
- \ref qref17
- \ref qref4
- \ref qref5
- \ref qref6
- \ref qref14
- \ref qref12
- \ref qref7
- \ref qref13
- \ref qref8
- \ref qref9
- \ref qref10
- \ref qref15
- \ref qref18
- \ref qref19

\n

\tableofcontents

<HR>
\section qref0 Defining a state machine

\n Prev: \ref qref "Quick reference" \n

A state machine application is defined with the RKH_SMA_CREATE() macro and 
declared with the RKH_SMA_DCLR() macro. Frequently, each state machine is 
encapsulated inside a dedicated source file (.c file), from which the 
RKH_SMA_CREATE() macro is used, thus the structure definition is in fact 
entirely encapsulated in its module and is inaccessible to the rest of the 
application. However, as a general rule, the state machine application must 
be declared inside a header file (.h file) by means of RKH_SMA_DCLR() 
macro. We will develop one example of state machine creation to illustrate 
the use of this macro. Also, we will give our hierarchical state machine the 
name \c my. If you wanted to create a "flat" state machine, you would use the 
#FLAT parameter rather than the #HCAL parameter.

This section includes:

- \ref qref0_1
- \ref qref0_2
- \ref qref0_3


\subsection qref0_1 Defining the state machine application
\n
\code
(1)	//	my.c: state machine application's module

(2) typedef struct
    {
		RKH_SMA_T sm;	// base structure
		rui8_t x;		// private member
		rui8_t y;		// private member
	} MYSM_T;

(3) static RKH_ROM_STATIC_EVENT( turnon, TURNON );

(4) RKH_SMA_CREATE( 	MYSM_T, 	// state machine application data type
(6) 					my, 		// name
(7)						2,			// priority number
(8) 					HCAL, 		// hierarchical state machine
(9) 					&S1, 		// initial state
(10) 					my_init, 	// initial action
(11) 					&turnon );	// initial event
\endcode

\subsection qref0_2 Declaring the state machine
\code
//	my.h: state-machine application's header file

RKH_SMA_DCLR( my );
\endcode

Explanation

\li (1)	Frequently, each state machine application is encapsulated inside a 
		dedicated source file (.c file), from which the RKH_SMA_CREATE() 
		macro is used.
\li (2)	The MYSM_T defines the \c my state machine application structure.
		Almost every state machine applications must also store 
		other "extended-state" information. You supply this additional 
		information by means of data members enlisted after the base structure 
		member \c sm. This illustrates how to derive an state machine 
		application from RKH_SMA_T. Please note that the RKH_SMA_T member 
		\c sm is defined as the FIRST member of the derived struct.
		RKH_SMA_T is not intended to be instantiated directly, but rather 
		serves as the base structure for derivation of state machines in the 
		application code.
\li (3)	Declares and initializes the event structure \c turnon with \c TURNON  
		signal and establishes it as one static event. The created event 
		object is explicitly placed in ROM..
\li (4)	As said below, the MYSM_T defines the \c my state machine application 
		structure.
\li (6)	\c my is the state machine application. Represents the top state of 
		state diagram. 
\li (7)	\c 2 is the state machine application priority.
\li (8)	the \c my state machine is defined as a hierarchical state machine. 
		The available property options are enumerated in RKH_HPPTY_T 
		enumeration in the \c rkh.h file.
\li (9)	\c S1 is the initial state.
\li (10) \c my_init() function defines the topmost initial transition in 
		the \c my state machine. 
		The function prototype is defined as RKH_TRN_ACT_T. This argument is 
		(optional), thus it could be declared as NULL. The application code 
		must trigger the initial transition explicitly by invoking 
		rkh_sma_activate() function.
\li (11) \c turnon is a pointer to an event that will be passed to state 
		machine application when it starts. Could be used to pass arguments 
		to the state machine like an argc/argv. This argument is optional, 
		thus it could be declared as NULL or eliminated in compile-time with 
		RKH_CFG_SMA_INIT_EVT_EN = 0.

\subsection qref0_3 Customization

Each RKH application must have its own configuration file, called 
\c rkhcfg.h. This file adapts and configures RKH by means of compiler
definitions and macros allowing to restrict the resources consumed by RKH.
Adjusting this definitions allows to reduce the ROM and RAM consumption,
and to enhance the system performance in a substantial manner. The 
\c rkhcfg.h shows the general layout of the configuration file.

Use the following macros to reduce the memory taken by state machine 
structure: #RKH_CFG_SMA_INIT_EVT_EN. 
See \ref cfg section for more information. 

Prev: \ref qref "Quick reference"

<HR>
\section qref1 Defining a composite state

\n Prev: \ref qref "Quick reference"

A superstate or composite state is defined with the RKH_CREATE_COMP_STATE()
macro and declared with the #RKH_DCLR_COMP_STATE macro. Frequently, each 
state machine and its states (composite and basic) are encapsulated 
inside a dedicated source file (.c file), from which the 
RKH_CREATE_COMP_STATE() macro is used.
We will develop one example of composite state definition to illustrate the 
use of this macro. We will give our composite state the name \c S1. 

This section includes:

- \ref qref1_1
- \ref qref1_2
- \ref qref1_3

\subsection qref1_1 Defining a composite state
\code
(1)	//	my.c: state-machine's module

(2)	RKH_CREATE_COMP_STATE( 	S1, 
(4)							turn_on, 
(5)							turn_off, 
(6)							&SA, 
(7)							&S11, 
(8)							&H );
\endcode

\subsection qref1_2 Declaring a composite state
\code
//	my.h: state-machine's header file

...
RKH_DCLR_COMP_STATE S1;
\endcode

Explanation

\li (1)	Frequently, each state machine and its states are encapsulated 
		inside a dedicated source file (.c file), from which the 
		RKH_CREATE_COMP_STATE() macro is used.
\li (2)	\c S1 is the state name. Represents a composite state structure.
\li (4)	\c turn_on() defines the entry action to be executed unconditionally 
		upon the entry to the \c S1 state. This argument is optional, 
		thus it could be declared as NULL. The RKH_ENT_ACT_T defines the function 
		prototype.
\li (5)	\c turn_off() defines the exit action, which is executed upon exit 
		from the \c S1 state. This argument is optional, thus it could be 
		declared as NULL. The RKH_EXT_ACT_T defines the function prototype.
\li (6)	\c SA is the parent state of \c S1. If a state has no 
		explicit superstate means that it is implicitly nested in 
		the "top" state, and the parent state is defined by means of RKH_ROOT
		macro.  The "top" state is a UML concept that denotes 
		the ultimate root of the state hierarchy in a hierarchical state 
		machine.
\li (7)	\c S11 is the default state of \c S1 state machine. At each level 
		of nesting, a superstate can have a private initial transition that
		designates the active substate after the superstate is entered 
		directly. Here the initial transition of state \c S1 designates the 
		state \c S11 as the initial active substate.
\li (8)	\c H is the history pseudostate. This argument is optional, thus 
		it could be declared as NULL. See RKH_CREATE_SHALLOW_HISTORY_STATE() 
		macro and RKH_CREATE_DEEP_HISTORY_STATE().

\subsection qref1_3 Customization

Each RKH application must have its own configuration file, called 
\c rkhcfg.h. This file adapts and configures RKH by means of compiler
definitions and macros allowing to restrict the resources consumed by RKH.
Adjusting this definitions allows to reduce the ROM and RAM consumption,
and to enhance the system performance in a substantial manner. The 
\c rkhcfg.h shows the general layout of the configuration file.

Use the following macros to reduce the memory taken by state machine 
structure: #RKH_CFG_SMA_HCAL_EN. 
See \ref cfg section for more information. 

Prev: \ref qref "Quick reference"

<HR>
\section qref2 Defining a basic state

\n Prev: \ref qref "Quick reference"

A basic state (also called substate) is defined with the 
RKH_CREATE_BASIC_STATE() macro and declared with the #RKH_DCLR_BASIC_STATE 
macro. Frequently, each state machine and its states (composite and 
basic) are encapsulated inside a dedicated source file (.c file), from 
which the RKH_CREATE_BASIC_STATE() macro is used.
We will develop one example of basic state definition to illustrate the 
use of this macro. We will give our basic state the name \c S11. 
As will demostrates the use of RKH_CREATE_BASIC_STATE() macro and its
arguments is very similar to RKH_CREATE_COMP_STATE() macro.

<b>Defining a basic state</b>
\code
(1)	//	my.c: state-machine's module

(2)	RKH_CREATE_BASIC_STATE( S11, 
(4)							start_process, 
(5)							stop_process, 
(6)							&S1, 
(7)							in_keyb );
\endcode

<b>Declaring a basic state</b>
\code
//	my.h: state-machine's header file

...
RKH_DCLR_BASIC_STATE S11;
\endcode

Explanation

\li (1)	Frequently, each state machine and its states are encapsulated 
		inside a dedicated source file (.c file), from which the 
		RKH_CREATE_BASIC_STATE() macro is used.
\li (2)	\c S11 is the state name. Represents a substate structure.
\li (4)	\c start_process() defines the entry action to be executed unconditionally 
		upon the entry to the \c S11 state. This argument is optional, 
		thus it could be declared as NULL. The RKH_ENT_ACT_T defines the function 
		prototype.
\li (5)	\c stop_process() defines the exit action, which is executed upon exit 
		from the \c S11 state. This argument is optional, thus it could be 
		declared as NULL. The RKH_EXT_ACT_T defines the function prototype.
\li (6)	\c S1 is the parent state of \c S11. If a state has no 
		explicit superstate means that it is implicitly nested in 
		the "top" state, and the parent state is defined by means of RKH_ROOT
		macro.  The "top" state is a UML concept that denotes 
		the ultimate root of the state hierarchy in a hierarchical state 
		machine.
\li (7)	\c in_keyb() defines the event preprocessor action. Before sending the 
		arrived event to state machine, it can be previously processed using 
		the	event preprocessor function. An action function takes the state 
		machine pointer and the event pointer as arguments. The first 
		parameter is optional in compile-time according to 
		\b RKH_EN_PPRO_HSM_ARG macro.
		Example:
\code
static
RKH_SIG_T
in_keyb( RKH_EVT_T *pe )
{
	if( pe->e >= 0 && pe->e <= 9 )
		return DECIMAL;
	if( pe->e == '.' )
		return POINT;
	else
		return pe->e;
}
\endcode

\li (7)	Aditionally, by means of single inheritance in C it could be 
		used to supply additional information to state.
		Implementing the single inheritance in C is very 
		simply by literally embedding the base type, RKH_PPRO_T in 
		this case, as the first member of the derived structure.
		This argument is optional, thus it could be declared as NULL.
		Example:
\code
typedef struct
{
	RKH_PPRO_T prepro; 	// extend the RKH_PPRO_T class
	unsigned min:4;		// addtional information...
	unsigned max:4;
	char *buff;
} SDATA_T;				// this data type will be used in S12 context

static const SDATA_T option = { preprocessor, 4, 8, token };

/* Define S11 state as a substate of S1 */
RKH_CREATE_BASIC_STATE( S11, start_process, start_process, &S1, in_keyb ); 

/* Define S12 state as a substate of S1 */
RKH_CREATE_BASIC_STATE( S12, NULL, NULL, &S1, (RKH_PPRO_T*)&option ); 
\endcode

\b Customization

Each RKH application must have its own configuration file, called 
\c rkhcfg.h. This file adapts and configures RKH by means of compiler
definitions and macros allowing to restrict the resources consumed by RKH.
Adjusting this definitions allows to reduce the ROM and RAM consumption,
and to enhance the system performance in a substantial manner. The 
\c rkhcfg.h shows the general layout of the configuration file.

Use the following macros to reduce the memory taken by state machine 
structure: RKH_CFG_SMA_PPRO_EN.
See \ref cfg section for more information. 

Prev: \ref qref "Quick reference"

<HR>
\section qrefsb Defining a submachine state

\n Prev: \ref qref "Quick reference"

A submachine state is a kind of a state that actually refers to 
another defined state machine. The diagram in following figure shows a 
fragment from a state machine diagram in which a submachine state 
(the \c SB) is referenced.

\anchor qr_fig_sbm1
\image html sbm1.png "Submachine state"

In the above example, the transition triggered by event \c TWO will 
terminate on entry point \c ENS12 of the \c SB state machine. 
The \c ONE transition implies taking of the default transition of the 
\c SB and executes the \c act5() action. The transition emanating from 
the \c EX1S12 exit point of the submachine will execute the \c act1() 
behavior in addition to what is executed within the \c SB state machine. 
Idem transition emanating from the \c EX2S12.
This transition must have been triggered within the \c SB state machine. 
Finally, the transition emanating from the edge of the submachine state 
is triggered by event \c THREE.

The following figure is an example of a state machine \c SB defined with 
two exit points, \c EXPNT1 and \c EXPNT2, and one entry point \c ENPNT.

\anchor qr_fig_sbm2
\image html sbm2.png "State machine with two exit points and one entry point"

In the following figure the state machine shown above is referenced twice in 
a submachine state \c S12 and \c S2.

\anchor qr_fig_sbm3
\image html sbm3.png "Submachine state with usage of exit and entry points"

This section includes:

- \ref qrefsb_1
- \ref qrefsb_2
- \ref qrefsb_3
- \ref qrefsb_4
- \ref qrefsb_5
- \ref qrefsb_6
- \ref qrefsb_7
- \ref qrefsb_8
- \ref qrefsb_9
- \ref qrefsb_10
- \ref qrefsb_11
- \ref qrefsb_c

\subsection qrefsb_1 Defining a submachine state

A submachine state is defined with the RKH_CREATE_SUBMACHINE_STATE()
macro and declared with the #RKH_DCLR_SUBM_STATE macro. Frequently, each 
state machine and its states (composites, basic, and submachine states) are 
encapsulated inside a dedicated source file (.c file), from which the 
RKH_CREATE_SUBMACHINE_STATE() macro is used.
We will develop one example of submachine state definition to illustrate the 
usage and its related macros. We will give our submachine state the name 
\c S12.

\code
(1)	//	my.c: state-machine's module

(2)	RKH_CREATE_SUBMACHINE_STATE( 	S12, 
(4)									NULL, 
(5)									NULL, 
(6)									&S1, 
(7)									&SB );
\endcode

Explanation

\li (1)	Frequently, each state machine and its states are encapsulated 
		inside a dedicated source file (.c file), from which the 
		RKH_CREATE_SUBMACHINE_STATE() macro is used.
\li (2)	\c S12 is the state name. Represents a submachine state structure.
\li (4)	the entry action is not used.
\li (5)	the exit action is not used.
\li (6)	\c S1 is the parent state of \c S12. If a state has no 
		explicit superstate means that it is implicitly nested in 
		the "top" state, and the parent state is defined by means of RKH_ROOT
		macro.  The "top" state is a UML concept that denotes 
		the ultimate root of the state hierarchy in a hierarchical state 
		machine.
\li (7)	\c SB is the defined submachine state machine, which is referenced by 
		\c S12 submachine state.

On the other hand, in RKH every submachine state is associated with a 
transition table, which is composed of a well-defined set of transitions. 
See \ref qref16 for defining a state transition table.

\code
RKH_CREATE_TRANS_TABLE( S12 )
	RKH_TRINT( TERM, 	NULL, 	terminate ),
	RKH_TRREG( THREE,	NULL, 	NULL, 	&S1 ),
RKH_END_TRANS_TABLE
\endcode

\subsection qrefsb_2 Declaring a submachine state
\code
//	my.h: state-machine's header file

...
RKH_DCLR_SUBM_STATE S12;
\endcode

\subsection qrefsb_3 Defining the exit connection references

Connection point references are sources/targets of transitions implying 
exits out of/entries into the submachine state machine referenced by a 
submachine state.
In RKH every submachine state is associated with a exit point connection 
point reference table, which is composed of a well-defined set of 
exit point connection references. Thus, in RKH each row in a exit point table 
references an exit point pseudostate as defined in the submachine of the 
submachine state that has the exit point connection point defined. 
Note that each row number matches with the index number of the exit point 
pseudostate that it represent.

\code
// --- exit point pseudostates of SB submachine ---
RKH_CREATE_REF_EXPNT( 	EXPNT1, 
						0,			// index of exit point connection table
						&SB );
RKH_CREATE_REF_EXPNT( 	EXPNT2, 
						1, 			// index of exit point connection table
						&SB );
...
// --- exit point connection references of S12 submachine state ---
(1) RKH_CREATE_EX_CNNPNT_TABLE( S12 )
(2) 	RKH_EX_CNNPNT( EX1S12, &EXPNT1, act1, &S13 ), // table index = 0 (EXPNT1)
(3) 	RKH_EX_CNNPNT( EX2S12, &EXPNT2, act2, &S3 ),  // table index = 1 (EXPNT2)
(4) RKH_END_EX_CNNPNT_TABLE
\endcode

Explanation

\li (1)	The RKH_CREATE_EX_CNNPNT_TABLE() macro creates the \c S12's 
		exit point connection reference table. Each exit point connection 
		reference table always begins with the macro 
		RKH_CREATE_EX_CNNPNT_TABLE() and ends with the macro 
		#RKH_END_EX_CNNPNT_TABLE. As noted above, sandwiched between these 
		macros are the exit point macros, RKH_EX_CNNPNT().

\li (2)	The RKH_EX_CNNPNT() macro defines the exit point connection 
		\c EX1S12, where \c EXPNT1 is the referenced exit point pseudostate, 
		\c act1() is the transition action function to be taken, and 
		\c S13 is the transition target state. The place in the table matches 
		with the \c EXPNT1's index field.

\li (3)	Idem (2).

\li (4)	The #RKH_END_EX_CNNPNT_TABLE macro ends exit point connection 
		reference table.

\subsection qrefsb_4 Defining the entry connection references

Connection point references are sources/targets of transitions implying 
exits out of/entries into the submachine state machine referenced by a 
submachine state.

A entry point connection reference is defined with the RKH_EN_CNNPNT() 
macro and declared with the #RKH_DCLR_ENPNT macro. 
Frequently, each state machine and its states and pseudostates are 
encapsulated inside a dedicated source file (.c file), from which the 
RKH_EN_CNNPNT() macro is used.
We will develop one example of entry point connection definition to 
illustrate the use of this macro. We will give our entry connection the 
name \c ENS12.

\code
(1)	//	my.c: state-machine's module

(2)	RKH_EN_CNNPNT( 	ENS12, 
					&ENPNT, 
					&S12 );
\endcode

Explanation

\li (1)	Frequently, each state machine and its states are encapsulated 
		inside a dedicated source file (.c file), from which the 
		RKH_EN_CNNPNT() macro is used.
\li (2)	The RKH_EN_CNNPNT() macro defines the exit point connection 
		\c ENS12, where \c ENPNT is the referenced entry point pseudostate, 
		and \c S12 is the transition target state.

\subsection qrefsb_5 Declaring entry connection references

\code
//	my.h: state-machine's header file

...
RKH_DCLR_ENPNT ENS12;
\endcode

\subsection qrefsb_6 Defining a submachine

A submachine is defined with the RKH_CREATE_REF_SUBMACHINE() macro and 
declared with the #RKH_DCLR_REF_SUBM macro. Frequently, each 
state machine and its states (composites, basic, and submachine states) are 
encapsulated inside a dedicated source file (.c file), from which the 
RKH_CREATE_REF_SUBMACHINE() macro is used.
We will develop one example of submachine definition to illustrate the 
usage and its related macros. We will give our submachine the name \c SB.

\code
(1)	//	my.c: state-machine's module

(2)	RKH_CREATE_REF_SUBMACHINE( 	SB, 
(4)								&SB1, 
(5)								NULL );
\endcode

Explanation

\li (1)	Frequently, each state machine and its states are encapsulated 
		inside a dedicated source file (.c file), from which the 
		RKH_CREATE_REF_SUBMACHINE() macro is used.
\li (2)	\c SB is the submachine name. Represents a submachine structure.
\li (4) \c SB1 is the default state of \c SB submachine. At each level of 
		nesting, a submachine can have a private initial transition that 
		designates the active substate after the submachine is entered 
		directly. Here the initial transition of state \c SB designates 
		the state \c SB1 as the initial active substate. 
\li (5) the initialization action is not used. 

\subsection qrefsb_7 Declaring a submachine

\code
//	my.h: state-machine's header file

...
RKH_DCLR_REF_SUBM SB;
\endcode

\subsection qrefsb_8 Defining the exit point

A exit point pseudostate is defined with the RKH_CREATE_REF_EXPNT() 
macro and declared with the #RKH_DCLR_REF_EXPNT macro. 
Frequently, each state machine and its states and pseudostates are 
encapsulated inside a dedicated source file (.c file), from which the 
RKH_CREATE_REF_EXPNT() macro is used.
We will develop one example of exit point definition to 
illustrate the use of this macro. We will give our exit points the 
name \c EXPNT1 and \c EXPNT2.

\code
(1)	//	my.c: state-machine's module

(2)	RKH_CREATE_REF_EXPNT( 	EXPNT1, 
							0, 
							&SB );
	RKH_CREATE_REF_EXPNT( 	EXPNT2, 
							1, 
							&SB );
\endcode

Explanation

\li (1)	Frequently, each state machine and its states are encapsulated 
		inside a dedicated source file (.c file), from which the 
		RKH_EN_CNNPNT() macro is used.
\li (2)	The RKH_CREATE_REF_EXPNT() macro defines the exit point \c EXPNT1, 
		where \c 0 is the index of the exit point connection table, and 
		\c SB is the containing submachine. See \ref qrefsb_5 for referencing 
		a submachine exit point from a submachine state.

\subsection qrefsb_9 Declaring a submachine

\code
//	my.h: state-machine's header file

...
RKH_DCLR_REF_EXPNT EXPNT1, EXPNT2;
\endcode

\subsection qrefsb_10 Defining the entry point

A entry point pseudostate is defined with the RKH_CREATE_REF_ENPNT() 
macro and declared with the #RKH_DCLR_REF_ENPNT macro. 
Frequently, each state machine and its states and pseudostates are 
encapsulated inside a dedicated source file (.c file), from which the 
RKH_CREATE_REF_ENPNT() macro is used.
We will develop one example of entry point definition to 
illustrate the use of this macro. We will give our entry point the 
name \c ENPNT.

\code
(1)	//	my.c: state-machine's module

(2)	RKH_CREATE_REF_ENPNT( 	ENPNT, 
							act8,
							&SB1,
							&SB );
\endcode

Explanation

\li (1)	Frequently, each state machine and its states are encapsulated 
		inside a dedicated source file (.c file), from which the 
		RKH_CREATE_REF_ENPNT() macro is used.
\li (2)	The RKH_CREATE_REF_ENPNT() macro defines the entry point \c ENPNT, 
		where \c activate() is the transition action to be taken, \c SB1 is 
		the transition target state, and \c SB is the containing submachine.

\subsection qrefsb_11 Declaring a entry point

\code
//	my.h: state-machine's header file

...
RKH_DCLR_REF_ENPNT ENPNT;
\endcode

\subsection qrefsb_c Customization

Each RKH application must have its own configuration file, called 
\c rkhcfg.h. This file adapts and configures RKH by means of compiler
definitions and macros allowing to restrict the resources consumed by RKH.
Adjusting this definitions allows to reduce the ROM and RAM consumption,
and to enhance the system performance in a substantial manner. The 
\c rkhcfg.h shows the general layout of the configuration file.

Use the following macros to reduce the memory taken by state machine 
structure: RKH_CFG_SMA_HCAL_EN, RKH_CFG_SMA_PSEUDOSTATE_EN, and RKH_CFG_SMA_SUBMACHINE_EN.
See \ref cfg section for more information. 

Prev: \ref qref "Quick reference"

<HR>
\section qref16 Defining a state transition table

\n Prev: \ref qref "Quick reference"

In RKH every state is associated with a transition table, which is composed 
of a well-defined set of transitions.
The general syntax of an expression labelling a transition in a statechart is
\e "i[c]/a" where \e i is the input that triggers the transition, \e c is a 
condition that guards the transition from being taken unless it is true when 
\e i occurs, and \e a is an action that is carried out if and when the 
transition is taken. All of these parts are optional. 
Thus, in RKH each row in a table represents a transition, which is 
well-defined by an \e event, a \e guard, an \e action, and \e target state 
(or pseudostate). The \ref fig_trtbl "Transition table" figure shows a 
demostrative example of that.

\anchor fig_trtbl
\image html trtbl.jpg "Transition table"

\li (1)	Expressing this transition as said above:\n\n
		<EM>RCV_DATA[ data == SYNC ]/store_data( data )</EM>\n\n
		The \c data \c == \c SYNC expression guards the transition from being 
		taken unless it is true. The \c store_data() function is the action to
		be taken. Note that in RKH the guard condition is evaluated invoking 
		to a function.

\li	(2)	Expressing this transition as said above:\n\n
		<EM>TOUT0/drop_frame()</EM>\n\n
		This transition is not conditioned by a guard.

\li	(3)	Expressing this transition as said above:\n\n
		<EM>DISABLE/disable()</EM>\n\n
		Similar to prior transition.


Now, is time to represent IN_DATA's transition table by means of RKH 
framework:

\code
(1)	RKH_CREATE_TRANS_TABLE( IN_DATA )
(2)		RKH_TRINT( RCV_DATA,	is_sync,	store_data 				),
(3)		RKH_TRREG( TOUT0, 		NULL, 		drop_frame, &WAIT_SYNC	),
		RKH_TRREG( DISABLE, 	NULL, 		disable, 	&IDLE		)
(4)	RKH_END_TRANS_TABLE
\endcode

Explanation

\li (1)	The RKH_CREATE_TRANS_TABLE() macro creates the \c IN_DATA's 
transition table. Each transition table always begins with the macro 
RKH_CREATE_TRANS_TABLE() and ends with the macro RKH_END_TRANS_TABLE().
As noted above, sandwiched between these macros are the transitions macros 
that actually represent behavior of state.

\li (2)	The RKH_TRINT() macro defines a internal transition, where 
\c RCV_DATA is the triggering event, \c is_sync is the guard function, 
and \c store_data() is the action function to be taken.

\li (3)	The RKH_TRREG() macro defines a regular state transition, where 
\c TOUT0 is the triggering event, \c drop_frame() is the action function to 
be taken, and \c WAIT_SYNC is the target state. Note that it is not guarded.

\li (4)	The RKH_END_TRANS_TABLE() macro ends transition table.

As said above, the actions and guards in RKH framework are represented by 
functions. Thus, \c is_sync() could be declared as:

\code
rbool_t 
is_sync( RKH_EVT_T *pe )
{
	return pe->e == SYNC;
}
\endcode

\n Prev: \ref qref "Quick reference"

<HR>
\section qref3 Defining a conditional pseudostate

\n Prev: \ref qref "Quick reference"

A conditional pseudostate (also called juncton pseudostate) is defined with 
the RKH_CREATE_COND_STATE() macro and declared with the 
#RKH_DCLR_COND_STATE macro. Frequently, each state machine and its 
states and pseudostates are encapsulated inside a dedicated source file 
(.c file), from which the RKH_CREATE_COND_STATE() macro is used.
We will develop one example of conditional pseudostate 
(aka junction pseudostate) definition to illustrate the use of this macro. We 
will give our junction pseudostate the name \c C1. As will demostrates the 
use of RKH_CREATE_COND_STATE() macro and its arguments is very similar to 
RKH_CREATE_BASIC_STATE() macro.

<b>Defining a conditional pseudostate</b>
\code
(1)	//	my.c: state-machine's module

(2)	RKH_CREATE_COND_STATE( 	C1 );
\endcode

<b>Declaring a conditional pseudostate</b>
\code
//	my.h: state-machine's header file

...
RKH_DCLR_COND_STATE C1;
\endcode

Explanation

\li (1)	Frequently, each state machine and its states are encapsulated 
		inside a dedicated source file (.c file), from which the 
		RKH_CREATE_COND_STATE() macro is used.
\li (2)	\c C1 is the pseudostate name. Represents a conditional pseudostate 
		structure.

\b Customization

Each RKH application must have its own configuration file, called 
\c rkhcfg.h. This file adapts and configures RKH by means of compiler
definitions and macros allowing to restrict the resources consumed by RKH.
Adjusting this definitions allows to reduce the ROM and RAM consumption,
and to enhance the system performance in a substantial manner. The 
\c rkhcfg.h shows the general layout of the configuration file.

Use the following macros to reduce the memory taken by state machine 
structure: RKH_CFG_SMA_HCAL_EN, RKH_CFG_SMA_PSEUDOSTATE_EN, RKH_CFG_SMA_CONDITIONAL_EN, 
RKH_SMA_MAX_TR_SEGS. 
See \ref cfg section for more information. 

Prev: \ref qref "Quick reference"

<HR>
\section qref17 Defining a branch table

\n Prev: \ref qref "Quick reference"

A condition connector has one incoming transition and can have several 
outgoing transition segments called branches. Branches are labeled with 
guards that determine which one is to be actually taken. 
Since the condition connector is an OR connector, only one of the branches 
can be taken. Each condition connector can have one special branch with 
a guard labeled rkh_sm_else, which is taken if all the guards on the other 
branches are false. 
Branches cannot contain triggers, but in addition to a guard they may 
contain actions. A branch can enter another condition connector, thus 
providing for the nesting of branches.
In RKH branches are defined by the macro RKH_BRANCH().
The general syntax of an expression labelling a branch in a statechart is
\e "[c]/a" where \e c is a condition that guards the transition from being 
taken unless it is true, and \e a is an action that is carried out if and 
when the transition is taken. All of these parts are optional. 
Example:

\code
(1)	RKH_CREATE_BRANCH_TABLE( C1 )
(2)		RKH_BRANCH( power_ok, 	enable_process,	&S22	),
(3)		RKH_BRANCH( ELSE, 		abort,			&S4 	),
(4)	RKH_END_BRANCH_TABLE
\endcode

Explanation

\li (1)	The RKH_CREATE_BRANCH_TABLE() macro creates the \c C1's 
branch table. Each table type always begins with the macro 
RKH_CREATE_BRANCH_TABLE() and ends with the macro RKH_END_BRANCH_TABLE().
As noted above, sandwiched between these macros are the segment macros 
that actually comprise the condition connector.

\li (2)	The RKH_BRANCH() macro defines a branch segment, where 
\c power_ok() is the guard function, \c enable_process is the action function 
to be taken, and S22 is the target state.

\li (3)	If all the guards on the other branches are false \c abort() function 
will be invoked, and \c S4 will be the next state.

\li (4)	The RKH_END_BRANCH_TABLE() macro ends branch table.

As said above, the actions and guards in RKH framework are represented by 
functions.

\n Prev: \ref qref "Quick reference"

<HR>
\section qref4 Defining a shallow history pseudostate

\n Prev: \ref qref "Quick reference"

A shallow history pseudostate is defined with the 
RKH_CREATE_SHALLOW_HISTORY_STATE() macro and declared with the 
#RKH_DCLR_SHIST_STATE macro. Frequently, each state machine and its 
states and pseudostates are encapsulated inside a dedicated source file 
(.c file), from which the RKH_CREATE_SHALLOW_HISTORY_STATE() macro is used.
We will develop one example of shallow history pseudostate definition to 
illustrate the use of this macro. We will give our history pseudostate 
the name \c H1. As will demostrates the use of RKH_CREATE_SHALLOW_HISTORY_STATE() 
macro and its arguments is very similar to RKH_CREATE_COND_STATE() macro.

<b>Defining a shallow history pseudostate</b>
\code
(1)	//	my.c: state-machine's module

(2)	RKH_CREATE_SHALLOW_HISTORY_STATE(	H1, 
(3)										6,
(4)										&S1 );
\endcode

<b>Declaring a shallow history pseudostate</b>
\code
//	my.h: state-machine's header file

...
RKH_DCLR_DHIST_STATE H1;
\endcode

Explanation

\li (1)	Frequently, each state machine and its states are encapsulated 
		inside a dedicated source file (.c file), from which the 
		RKH_CREATE_SHALLOW_HISTORY_STATE() macro is used.
\li (2)	\c H1 is the pseudostate name. Represents a shallow history 
		pseudostate structure.
\li (3)	\c 6 is the value of pseudostate ID.
\li (6)	\c S1 is the parent state of \c H1.

\b Customization

Each RKH application must have its own configuration file, called 
\c rkhcfg.h. This file adapts and configures RKH by means of compiler
definitions and macros allowing to restrict the resources consumed by RKH.
Adjusting this definitions allows to reduce the ROM and RAM consumption,
and to enhance the system performance in a substantial manner. The 
\c rkhcfg.h shows the general layout of the configuration file.
Use the following macros to reduce the memory taken by state machine 
structure: RKH_CFG_SMA_HCAL_EN, RKH_CFG_SMA_PSEUDOSTATE_EN, RKH_SMA_MAX_TR_SEGS, 
RKH_EN_SHALLOW_HISTORY. 
See \ref cfg section for more information. 

Prev: \ref qref "Quick reference"

<HR>
\section qref5 Defining a deep history pseudostate

\n Prev: \ref qref "Quick reference"

A deep history pseudostate is defined with the 
RKH_CREATE_DEEP_HISTORY_STATE() macro and declared with the 
#RKH_DCLR_DHIST_STATE macro. Frequently, each state machine and its 
states and pseudostates are encapsulated inside a dedicated source file 
(.c file), from which the RKH_CREATE_DEEP_HISTORY_STATE() macro is used.
We will develop one example of deep history pseudostate definition to 
illustrate the use of this macro. We will give our history pseudostate 
the name \c H2. As will demostrates the use of 
RKH_CREATE_DEEP_HISTORY_STATE() macro and its arguments is very 
similar to RKH_CREATE_SHALLOW_HISTORY_STATE() macro.

<b>Defining a deep history pseudostate</b> \n
\code
(1)	//	my.c: state-machine's module

(2)	RKH_CREATE_DEEP_HISTORY_STATE(	H1, 
(4)									&S21 );
\endcode

<b>Declaring a shallow history pseudostate</b>
\code
//	my.h: state-machine's header file

...
RKH_DCLR_DHIST_STATE H1;
\endcode

Explanation

\li (1)	Frequently, each state machine and its states are encapsulated 
		inside a dedicated source file (.c file), from which the 
		RKH_CREATE_DEEP_HISTORY_STATE() macro is used.
\li (2)	\c H2 is the pseudostate name. Represents a deep history 
		pseudostate structure.
\li (6)	\c S21 is the parent state of \c H2.

\b Customization

Each RKH application must have its own configuration file, called 
\c rkhcfg.h. This file adapts and configures RKH by means of compiler
definitions and macros allowing to restrict the resources consumed by RKH.
Adjusting this definitions allows to reduce the ROM and RAM consumption,
and to enhance the system performance in a substantial manner. The 
\c rkhcfg.h shows the general layout of the configuration file.
Use the following macros to reduce the memory taken by state machine 
structure: RKH_CFG_SMA_HCAL_EN, RKH_CFG_SMA_PSEUDOSTATE_EN, RKH_SMA_MAX_TR_SEGS, 
RKH_EN_DEEP_HISTORY. 
See \ref cfg section for more information. 

Prev: \ref qref "Quick reference"

<HR>
\section qref6 Defining a choice pseudostate

\n Prev: \ref qref "Quick reference"

A choice pseudostate is defined with the RKH_CREATE_CHOICE_STATE() macro and 
declared with the #RKH_DCLR_CHOICE_STATE macro. Frequently, each state 
machine and its states and pseudostates are encapsulated inside a dedicated 
source file (.c file), from which the RKH_CREATE_CHOICE_STATE() macro is used.
We will develop one example of choice pseudostate definition to 
illustrate the use of this macro. We will give our choice pseudostate 
the name \c CH. As will demostrates the use of RKH_CREATE_CHOICE_STATE() 
macro and its arguments is very similar to RKH_CREATE_COND_STATE() macro.

<b>Defining a choice pseudostate</b> \n
\code
(1)	//	my.c: state-machine's module

(2)	RKH_CREATE_CHOICE_STATE( CH );
\endcode

<b>Declaring a choice pseudostate</b>
\code
//	my.h: state-machine's header file

...
RKH_DCLR_CHOICE_STATE	CH;
\endcode

Explanation

\li (1)	Frequently, each state machine and its states are encapsulated 
		inside a dedicated source file (.c file), from which the 
		RKH_CREATE_CHOICE_STATE() macro is used.
\li (2)	\c CH is the pseudostate name. Represents a choice pseudostate 
		structure.

\b Customization

Each RKH application must have its own configuration file, called 
\c rkhcfg.h. This file adapts and configures RKH by means of compiler
definitions and macros allowing to restrict the resources consumed by RKH.
Adjusting this definitions allows to reduce the ROM and RAM consumption,
and to enhance the system performance in a substantial manner. The 
\c rkhcfg.h shows the general layout of the configuration file.

Use the following macros to reduce the memory taken by state machine 
structure: RKH_CFG_SMA_HCAL_EN, RKH_CFG_SMA_PSEUDOSTATE_EN, RKH_CFG_SMA_CHOICE_EN, 
RKH_SMA_MAX_TR_SEGS. 
See \ref cfg section for more information. 

Prev: \ref qref "Quick reference"

<HR>
\section qref14 Defining entry, exit, and transition actions

\n Prev: \ref qref "Quick reference"

This section summarize the functions and its prototypes used by RKH 
framework. As mentioned before, the framework make use the callbacks, i.e. 
pointer to functions, in most of its data structures by means of 
RKH_SMA_CREATE(), RKH_CREATE_COMP_STATE(), 
RKH_CREATE_BASIC_STATE(), RKH_TRINT(), RKH_TRREG(), RKH_BRANCH(), 
RKH_CREATE_SHALLOW_HISTORY_STATE(), RKH_CREATE_DEEP_HISTORY_STATE(), and 
RKH_CREATE_CHOICE_STATE() macros.
Obviously, the set of available functions and its configurations 
is mandatory to known for properly using the framework. 

<b>Initialization action</b>

\copydetails RKH_TRN_ACT_T

As said above, the application must explicitly trigger initial transitions 
in all state machines. The following listing shows the use of 
rkh_sma_activate() function, when it's is called the initial action is 
invoked and the state machine start. The rkh_sma_activate() is a 
platform-dependent function. All RKH ports must be defined in the RKH port 
file to a particular platform. However, only the 
ports to the external OS/RTOS usually need some code to bolt the framework to 
the external OS/RTOS.

\code
// start the manager state machine application
rkh_sma_activate( 	manager, 		// state machine application (SMA)
					qmgr, 			// event storage area for manager SMA
					QMGR_SIZE, 		// size of the storage area
					( void * )0, 	// not used
					0 );			// not used
\endcode

The next listing shows an example of the initial action implementation.

\code
void 
manager_init( const struct rkh_t *sma, RKH_EVT_T *pe )
{
	dprint( "Init \"manager\" state machine\n" );
	manager_turnon();	
}
\endcode

<b>Exit action</b>

\copydetails RKH_EXT_ACT_T

The next listing shows an example of the exit action implementation.

\code
void 
idle_exit( const struct rkh_t *sma )
{
	dprint( "Exit from \"IDLE\" state\n" );
	manager_restore();
}
\endcode

<b>Entry action</b>

\copydetails RKH_ENT_ACT_T

The next listing shows an example of the entry action implementation.

\code
void 
wait_process_entry( const struct rkh_t *sma )
{
	dprint( "Entry to \"WAIT_PROCESS\" state\n" );
	manager_deactivate();
}
\endcode

<b>Transition action</b>

\copydetails RKH_TRN_ACT_T

The next listing shows an example of the transition action implementation.

\code
void 
set_config( const struct rkh_t *sma, RKH_EVT_T *pe )
{
	MYEVT_T *e;

	(void)sma;		/* argument not used */
	(void)pe;		/* argument not used */

	e = RKH_ALLOC_EVT( MYEVT_T, SIX, sma );
	e->ts = ( rui16_t )rand();
	RKH_TMR_ONESHOT( &my_timer, sma, MY_TICK );
}
\endcode

<b>Event preprocessor</b>

\copydetails RKH_PPRO_T

The next listing shows an example of the event preprocessor 
action implementation.

\code
RKH_SIG_T 
preprocess_keys( const struct rkh_t *sma, RKH_EVT_T *pe )
{
    if( pe->e >= 0 && pe->e <= 9 )
        return DECIMAL;
    if( pe->e == '.' )
        return POINT;
    else
        return pe->e;
}
\endcode

<b>Guard</b>

\copydetails RKH_GUARD_T

The next listing shows an example of the guard function implementation.

\code
rbool_t 
is_zero( const struct rkh_t *sma, RKH_EVT_T *pe )
{
	return get_water_level( CHANNEL( (( CHEVT_T* )pe)->ch ) ) == 0;
}
\endcode

\n Prev: \ref qref "Quick reference"

<HR>
\section qref12 Using events with parameters

\n Prev: \ref qref "Quick reference"

An event can have associated parameters, allowing the event 
instance to convey not only the occurrence of some interesting 
incident but also quantitative information regarding that occurrence.
Implementing the single inheritance in C is very simply by literally
embedding the base structure, RKH_EVT_T in this case, as the first 
member of the derived structure.
For example, the structure MYEVT_T derived from the base structure 
RKH_EVT_T by embedding the RKH_EVT_T instance as the first member of 
MYEVT_T.
See also, \ref qref7 section for more information.

\code
typedef struct
{
	RKH_EVT_T evt;	// base structure
	int x;			// parameter 'x'
	int y;			// parameter 'y'
} MYEVT_T;
\endcode

Such nesting of structures always aligns the data member 'evt' at the 
beginning of every instance of the derived structure. In particular, this 
alignment lets you treat a pointer to the derived MYEVT_T structure as a 
pointer to the RKH_EVT_T base structure. Consequently, you can always 
safely pass a pointer to MYEVT_T to any C function that expects a pointer 
to RKH_EVT_T. (To be strictly correct in C, you should explicitly cast this 
pointer. In OOP such casting is called upcasting and is always safe.) 
Therefore, all functions designed for the RKH_EVT_T structure are 
automatically available to the MYEVT_T structure as well as other 
structures derived from RKH_EVT_T.

The RKH takes the \a 'e' member of RKH_EVT_T structure for triggering a 
state transition.

See also rkh_sma_put_fifo(), rkh_sma_put_lifo(), RKH_ALLOC_EVT(), 
RKH_SET_STATIC_EVENT(), and RKH_FWK_GC().

\n Prev: \ref qref "Quick reference"

<HR>
\section qref7 Using dynamic and static events

\n Prev: \ref qref "Quick reference"

In RKH as other frameworks, the actual event instances are either constant 
events (or static events) statically allocated at compile time or dynamic events
allocated at runtime from one of the event pools that the framework manages.

This section includes:

- \ref qref7_1
- \ref qref7_2
- \ref qref7_3
- \ref qref7_4
- \ref qref7_5

\subsection qref7_1 Registering the event pool

Before using dynamic events (or event with arguments) the application code 
must register the proper event pools, which stores the events as a 
fixed-sized memory block. 
Each event pool must be registered with the RKH framework, by means of the 
rkh_fwk_epool_register() function. This function initializes one event pool at a 
time and must be called exactly once for each event pool before the pool can 
be used.

The application code might initialize the event pools by making calls 
to the rkh_fwk_epool_register() function. However, for the simplicity of 
the internal implementation, the application code initialize event pools 
in the ascending order of the event size.

\code
#define SIZEOF_EP0STO				64
#define SIZEOF_EP0_BLOCK			sizeof( TOUT_T )

#define SIZEOF_EP1STO				32
#define SIZEOF_EP1_BLOCK			sizeof( DIAL_T )

#define SIZEOF_EP2STO				32
#define SIZEOF_EP2_BLOCK			sizeof( SETUP_T )

typedef struct
{
	RKH_EVT_T evt;                   // base structure
	int timerno;					// parameter 'timerno'
} TOUT_T;

typedef struct
{
	RKH_EVT_T evt;                   // base structure
	char dial[ MAX_SIZE_DIAL ];     // parameter 'dial'
	int qty;                        // parameter 'qty'
} DIAL_T;
	
typedef struct
{
	RKH_EVT_T evt;                   // base structure
	int volume;                     // parameter 'volume'
	int baud_rate;                  // parameter 'baud_rate'
	char name[ MAX_SIZE_NAME ];     // parameter 'name'
	int iloop;                      // parameter 'iloop'
} SETUP_T;

// declares the storage memory of event pool
static rui8_t	ep0sto[ SIZEOF_EP0STO ],
				ep1sto[ SIZEOF_EP1STO ],
				ep2sto[ SIZEOF_EP2STO ];

...
rkh_fwk_epool_register( ep0sto, SIZEOF_EP0STO, SIZEOF_EP0_BLOCK  );
rkh_fwk_epool_register( ep1sto, SIZEOF_EP1STO, SIZEOF_EP1_BLOCK  );
rkh_fwk_epool_register( ep2sto, SIZEOF_EP2STO, SIZEOF_EP2_BLOCK  );
...
\endcode

\subsection qref7_2 Allocating events

\code
(1)	typedef struct
	{
		RKH_EVT_T evt;					// base structure
		char dial[ MAX_SIZE_DIAL ];		// parameter 'dial'
		int qty;						// parameter 'qty'
	} DIAL_T;

(2)	typedef struct
	{
		RKH_EVT_T evt;					// base structure
		int volume;						// parameter 'volume'
		int baud_rate;					// parameter 'baud_rate'
		char name[ MAX_SIZE_NAME ];		// parameter 'name'
		int iloop;						// parameter 'iloop'
	} SETUP_T;

	typedef struct
	{
		RKH_EVT_T evt;
		int timerno;
	} TOUT_T;

(3) static RKH_ROM_STATIC_EVENT( offh, OFFHOOK );
(4) static TOUT_T tout;

...

(5) RKH_SET_STATIC_EVENT( &tout, TIMEOUT );
(6)	DIAL_T *de = RKH_ALLOC_EVT( DIAL_T, DIALED, me );
(7)	SETUP_T *se = RKH_ALLOC_EVT( SETUP_T, SET_CONFIG, me );
(8)	se->volume = 0;
	se->baud_rate = DEFAULT_BAUD_RATE;
	se->iloop = 2;
\endcode

Explanation

\li (1-2)	As mentioned before, implementing the single inheritance in C is 
			very simply by literally embedding the base structure, RKH_EVT_T 
			in this case, as the first member of the derived structure, 
			\c DIAL_T and \c SETUP_T.
\li (3)		The \c OFFHOOK event never changes, so it can be statically 
			allocated just once. This event is declared as const, which 
			means that it can be placed in ROM. The initializer list for 
			this event consists of the signal \c OFFHOOK followed by zero. 
			This zero informs the RKH framework that this event is 
			<em>static</em> and should never be recycled to an event pool.
\li	(4)		The <c>TIMEOUT( timerno )</c> event is an example of an event with 
			changing parameters. In general, such an event cannot be allocated 
			in ROM like the \c OFFHOOK event because it can change. 
\li (5)		This macro set the event <c>TIMEOUT( timerno )</c> with \c TIMEOUT 
			signal and establishes it as one <em>static event</em>.
\li (6-7) 	The RKH_ALLOC_EVT() macro dynamically creates a new instances 
			events of type \c DIAL_T and \c SETUP_T with \c DIALED and 
			\c SET_CONFIG signals. These events are represented like this:
			<c>DIALED( dial, qty )</c> and 
			<c> SET_CONFIG( volume, baud_rate, name, iloop )</c>
			This macro returns a pointer to the event already cast to the 
			required event type.
\li (8) 	The \c volume, \c baud_rate, and \c iloop parameters of the event 
			are assigned.

Just one another example that it could be used to easily debug an application 
with static events.

\code
typedef struct
{
	RKH_EVT_T evt;
#if SYSEVT_DEBUG == 1
	char *name;
#endif
} SYSEVT_T;

#if SYSEVT_DEBUG == 1
	#define mkse( e, n )		{ { (e), 0, 0 }, (n) }
#else
	#define mkse( e, n )		{ { (e), 0, 0 } }
#endif

static const SYSEVT_T sysevts[] =
{
	mkse( OFFHOOK,	"offhook" ),
	mkse( ONHOOK, 	"onhook" ),
	mkse( POLREV, 	"polarity reversal" ),
	mkse( BTONE,	"billing tone" ),
	...
};
\endcode

\subsection qref7_3 Posting events

The RKH framework supports one type of asynchronous event exchange: the 
simple mechanism of direct event posting supported through the functions
rkh_sma_post_fifo() and rkh_sma_post_lifo(), when the producer of an event 
directly posts the event to the event queue of the consumer SMA 
(active object).

\code
(1) static RKH_ROM_STATIC_EVENT( eterm, TERM );
...

(2)	rkh_sma_post_fifo( manager, &eterm );
\endcode

\li (1)	Declares and initializes the event \a eterm with \a TERM 
		signal and establishes it as one static event.
\li (2)	The \c eterm event is sent directly to the \c manager SMA.

\code
...
(1) mye = RKH_ALLOC_EVT( MYEVT_T, kbmap( c ), me );
(2) mye->ts = ( rui16_t )rand();
(3) rkh_sma_post_fifo( my, ( RKH_EVT_T* )mye );
\endcode

\li (1)	Dynamically creates a new event \c mye of type \a MYEVT_T with 
		the signal returned from the kbmap() function.
\li (2) Use the extended members of the event \c mye.
\li (3) The \c mye event is sent directly to the \c my SMA.

\subsection qref7_4 Recycling dynamic events

If the system make use of dynamic events facility after the processing, 
you must not forget to call the RKH garbage collector, because now RKH is 
no longer in charge of event processing and you are solely responsible for 
not leaking the event.
The garbage collector actually recycles the event only when it determines 
that the event is no longer referenced.
The following listing illustrates how and when to invoke RKH_FWK_GC() macro 
to recycle "dynamic" events.

\code
	void 
	rkh_fwk_enter( void )
    {
		rui8_t prio;
		RKH_SMA_T *sma;
		RKH_EVT_T *e;

		rkh_hook_start();
		RKH_TR_FWK_EN();

		FOREVER
		{
			RKH_DIS_INTERRUPT();
			if( RKH_RDY_ISNOT_EMPTY( rkhrg ) )
			{
				RKH_RDY_FIND_HIGHEST( rkhrg, prio );
				sma = rkh_sptbl[ prio ];
				RKH_ENA_INTERRUPT();

(1)				e = rkh_sma_get( sma );
(2)				rkh_sm_dispatch( (RKH_SM_T *)sma, e );
(3)				RKH_FWK_GC( e );
			}
			else 
			//
			// rkh_hook_idle() must be called with interrupts DISABLED because the 
			// determination of the idle condition (no events in the queues) can 
			// change at any time by an interrupt posting events to a queue. The 
			// rkh_hook_idle() MUST enable interrups internally, perhaps at the 
			// same time as putting the CPU into a power-saving mode.
			//			
				rkh_hook_idle();
		}
    }
\endcode

\li (1)	An event \c e is get from the SMA queue with the highest priority.
\li (2)	The event \c e is dispatched to the current SMA.
\li	(2) Thus, the event \c e is passed to the RKH garbage collector for 
		recycling. As described above, the RKH_FWK_GC() macro actually recycles 
		the wvent only when it determines that the event is no longer 
		referenced.

\subsection qref7_5 Customization

Each RKH application must have its own configuration file, called 
\c rkhcfg.h. This file adapts and configures RKH by means of compiler
definitions and macros allowing to restrict the resources consumed by RKH.
Adjusting this definitions allows to reduce the ROM and RAM consumption,
and to enhance the system performance in a substantial manner. The 
\c rkhcfg.h shows the general layout of the configuration file.
Use the following macros to reduce the memory taken by state machine 
structure: RKH_CFG_FWK_DYN_EVT_EN, RKH_CFG_FWK_MAX_EVT_POOL, RKH_CFG_FWK_SIZEOF_EVT, 
RKH_CFG_FWK_SIZEOF_EVT_SIZE, RKH_CFGPORT_NATIVE_DYN_EVT_EN, RKH_DYNE_TYPE, 
RKH_DYNE_INIT, RKH_DYNE_GET_ESIZE, RKH_DYNE_GET, RKH_DYNE_PUT, 
RKH_DYNE_GET_NFREE, RKH_DYNE_GET_NMIN, RKH_DYNE_GET_PSIZE.
See \ref cfg section for more information. 

Prev: \ref qref "Quick reference"

<HR>
\section qref13 Preprocessing events before of dispatch it

\n Prev: \ref qref "Quick reference"

\copydetails RKH_PPRO_T

The next listing shows an example of the event preprocessor 
action implementation.

\code
RKH_SIG_T 
preprocess_keys( const struct rkh_t *sma, RKH_EVT_T *pe )
{
	printf( "From state machine \"%s\"\n", rkh_get_sm_name( sma ) );
    if( pe->e >= 0 && pe->e <= 9 )
        return DECIMAL;
    if( pe->e == '.' )
        return POINT;
    else
        return pe->e;
}
\endcode

\n Prev: \ref qref "Quick reference"

<HR>
\section qref8 Deferring and recalling events

\n Prev: \ref qref "Quick reference"

Event deferral comes in very handy when an event arrives in a 
particularly inconvenient moment but can be deferred for some later time, 
when the system is in a much better position to handle the event. RKH 
supports very efficient event deferring and recalling mechanisms.
This function is part of the event deferral mechanism. An SMA 
uses this function to defer an event \a e to the event queue \a q. 
RKH correctly accounts for another outstanding reference to the event 
and will not recycle the event at the end of the RTC step. 
Later, the SMA might recall one event at a time from the 
event queue by means of rkh_sma_recall(sma,q) function. 
Recalling an event means that it is removed from the deferred event 
queue \c q and posted (LIFO) to the event queue of the \c sma state 
machine application.

This section includes:

- \ref qref8_1
- \ref qref8_2

\subsection qref8_1 Deferring an event

\code
static RKH_QUEUE_T qurc;
static RKH_EVT_T *qurc_sto[ MAX_SIZEOF_QURC ];

(1) rkh_queue_init( &qurc, (const void **)qurc_sto, MAX_SIZEOF_QURC, NULL );
...

void 
ring( const struct rkh_t *sma, RKH_EVT_T *pe )
{
	(void)sma;      				// argument not used
(2)	rkh_sma_defer( &qurc, pe );
}
\endcode

Explanation

\li (1)	Initialize the \c qurc queue to be used as deferred queue. 
\li (1)	Defers the event \c pe to the previously created event queue \c qurc. 

\subsection qref8_2 Recalling an event

\code
void 
exit_rx_manager( const struct rkh_t *sma )
{
(1)	rkh_sma_defer( sma, &qurc );
}
\endcode

Explanation

\li (1)	Removes an event from the deferred event queue \c qurc and posts (LIFO) 
		to the event queue of the \c sma state machine application.

\n Prev: \ref qref "Quick reference"

<HR>
\section qref9 Using assertions

\n Prev: \ref qref "Quick reference"

\copydetails rkhassert.h

\n Prev: \ref qref "Quick reference"

<HR>
\section qref10 Debugging an application based on RKH's state machines

\n Prev: \ref qref "Quick reference"

Please refer to \ref dbg section. 

\n Prev: \ref qref "Quick reference"

<HR>
\section qref15 Initializing a state machine and dispatching events

\n Prev: \ref qref "Quick reference"

The following listing shows an very simple example of the main() function 
implementation and demostrates how to use the RKH API.

\note
See \ref Usage "Getting started with RKH" section 
for more information about this.

\code
	...
	typedef struct
	{
		RKH_EVT_T event;
		rkhuint16 ts;
	} MYEVT_T;

	static MYEVT_T mye;
	...

	int
	main( int argc, char *argv[] )
	{
		int c;

(1)		rkh_trc_open();
(2)		rkh_fwk_init();

		srand( ( unsigned )time( NULL ) );
(3)  	rkh_sm_init( (RKH_SM_T *)my );

(4)		FOREVER
		{
(5)			c = mygetch();
			
			if( c == 'p' )
(6)				rkh_trc_flush();
			else if ( c == ESC )
			{
(7)				rkh_sm_dispatch( (RKH_SM_T *)my, &term );
				break;
			}
			else
			{
(8)				mye = RKH_ALLOC_EVT( MYEVT_T, kbmap( c ), 0 );
(9)				mye->ts = ( rui16_t )rand();
(10)			rkh_sm_dispatch( (RKH_SM_T *)my, ( RKH_EVT_T* )mye );
			}
		}

(11)	rkh_trc_close();
(12)	rkh_fwk_exit();
	}
\endcode

\li (1) Open the trace session.
\li (2) Initialize the framework RKH.
\li (3) Initialize the \c my state machine. 
		RKH invokes the defined init action.
\li (4) This is the event loop of the framework RKH.
\li (5) Gets key pressed from the standard input.
\li (6) Flush the trace stream.
\li (7) Send the \c term event to \c my state machine to terminate. 
		After that, terminates the program.
\li (8) Allocates an event of \c MYEVT_T type (derived from RKH_EVT_T) to 
		store the key pressed.
\li (9)	The	event has associated parameters that convey a random number.
\li (10) The \c mye event is dispatched to \c my state machine. Events with 
		parameters, such as the MYEVT_T, require explicit casting from the 
		generic base structure #RKH_EVT_T to the specific derived structure 
		MYEVT_T.
\li (11-12) Close the trace session and terminates the program.

<HR>
\section qref18 Using software timers

\n Prev: \ref qref "Quick reference" \n

\copydetails rkhtmr.h

This section includes:

- \ref qref18_1
- \ref qref18_2
- \ref qref18_3

\subsection qref18_1 Declaring, and initializing a timer

\code
    #define SYNC_TIME    RKH_TIME_MS(100)
    ...
(1) static RKHTmEvt tmSync;
    ...
    void
    powerOn(ActiveObject *const me)
    {
(2)     RKH_SET_STATIC_EVENT(&tmSync, Sync);
        RKH_TMR_INIT(&tmSync.tmr, 
                     RKH_UPCAST(RKH_EVT_T, &tmSync), 
                     NULL);
(3)     RKH_TMR_ONESHOT(&tmSync.tmr, 
                        RKH_UPCAST(RKH_SMA_T, me), 
                        SYNC_TIME);
    }
\endcode

Explanation

\li (1)	Declares and allocates the \c tmSync time event. 
\li (2)	Initializes \c tmSync like a static time event with signal \c Sync. 
        It should be used like a transition's trigger to support the UML's
        time event \c 'after \c SYNC_TIME':
        \code
        RKH_CREATE_BASIC_STATE(s1, powerOn, powerOff, RKH_ROOT, NULL);
        RKH_CREATE_TRANS_TABLE(s1)
            RKH_TRREG(Sync, NULL, NULL, &s0), /* 'Sync' signal enables this */
                                              /* transition to 's0' state */
                                              /* It is equivalent to */
                                              /* 'after SYNC_TIME' UML's TimeEvent */
        RKH_END_TRANS_TABLE
        \endcode
\li (3)	Initializes the \c tmSync timer as oneshot. When it expires the 
        \c me active object will receive an event with \c Sync signal.
        The \c tmSync will expire after \c SYNC_TIME ticks.

\subsection qref18_2 Start and stop timers

\code
(1)	#define TPWROFF_TIME    RKH_TIME_MS(100)
(2)	#define TKEY_TIME		RKH_TIME_MS(100)
   	#define TKEYSTART_TIME	RKH_TIME_MS(300)
    ...
(3) static RKHTmEvt tPwrOff,
(4) 			    tKey;
    ...
    void
    start(PowerMonitor *const me)
    {
(5)     RKH_TMR_ONESHOT(&tPwrOff.tmr, 
                        RKH_UPCAST(RKH_SMA_T, me), 
                        TPWROFF_TIME);
    }
    ...
    void
    close(PowerMonitor *const me)
    {
(6)     RKH_TMR_PERIODIC(&tKey.tmr, 
                         RKH_UPCAST(RKH_SMA_T, me), 
                         TKEYSTART_TIME,
                         TKEY_TIME);
    }
    ...
    void
    detect(PowerMonitor *const me)
    {
(7)     rkh_tmr_stop(&tKey);
(8)     rkh_tmr_stop(&tPwrOff);
    }
\endcode

Explanation

\li (1-2) Defines the number of millisencods for timer expiration using 
		  the macro RKH_TIME_MS(), which converts ticks to time. 
\li (3-4) Declares and allocates the \c tPwrOff, and \c tKey event timers. 
\li (5)	Starts the previously initialized \c tPwrOff event timer as oneshot 
        timer, which posts the \c evPwrOff signal to \c PowerMonitor active 
        object after \c TPWROFF_TIME ticks.
\li (6) Starts the previously initialized \c tKey event timer as periodic 
        timer, which posts the \c evKey signal to \c PowerMonitor active 
        object after \c TKEYSTART_TIME ticks initially and then after 
        every \c TKEY_TIME ticks.
\li (7-8) Stops both event timers. 

\subsection qref18_3 Customization

Each RKH application must have its own configuration file, called 
\c rkhcfg.h. This file adapts and configures RKH by means of compiler
definitions and macros allowing to restrict the resources consumed by RKH.
Adjusting this definitions allows to reduce the ROM and RAM consumption,
and to enhance the system performance in a substantial manner. The 
\c rkhcfg.h shows the general layout of the configuration file.
Use the following macros to reduce the memory taken by state machine 
structure: #RKH_CFG_TMR_EN, #RKH_CFG_TMR_SIZEOF_NTIMER, #RKH_CFG_TMR_HOOK_EN, 
#RKH_CFG_TMR_GET_INFO_EN.
See \ref cfg section for more information. 

<HR>
\section qref19 Defining special active objects

\n Prev: \ref qref "Quick reference" \n

\n
This section presents a series of examples that demonstrate the RKH framework's 
ability to implement different types of active objects, including periodic and 
non-reactive ones, showcasing its flexibility. In this case, the word type 
refers to object behavior. According to the behavioral classification scheme 
proposed by Bruce Douglass, it may be simple (its behavior is not dependent on 
its history), reactive (its behavior depends on a finite set of states), or 
continuous (its behavior depends on its history but in a continuous way).

-# \ref qref19_1
-# \ref qref19_2
-# \ref qref19_3
    -# \ref qref19_3_1
        -# \ref qref19_3_1_1
        -# \ref qref19_3_1_2
    -# \ref qref19_3_2
        -# \ref qref19_3_2_1
        -# \ref qref19_3_2_2
-# \ref qref19_customization

\subsection qref19_1 Active object with a parameterized state machine

In order to demonstrate how to handle a parameterized state machine from an 
active object by using the RKH framework, a digital pulse counter will be 
used as an example. It validates and counts digital pulses from N digital 
signals of the same type, whose waveforms and its parameters are shown below.
The behavior of the pulse counters are modeled as statecharts, but they are 
not active objects. Instead, pulse counters called PulseCounters are 
components of a container active object called PulseCounterMgr. The container 
is entirely responsible for its components. In particular, it must explicitly 
trigger initial transitions in all components as well as explicitly dispatch 
events to its components. They share both event queue and priority level of 
its container.
The entire example is located in doc/examples/PulseCounterMgr directory.

-# \ref qref19_1_1
-# \ref qref19_1_2
-# \ref qref19_1_3
-# \ref qref19_1_4
-# \ref qref19_1_5
-# \ref qref19_1_6
-# \ref qref19_1_7

\subsubsection qref19_1_1 Define the type of the state machine to be parameterized
The parameterized state machine is represented as a statechart. Almost every 
state machine must also store other extended information, which is maintained 
by means of data members enlisted after the base structure, in this case 
RKH_SM_T. Moreover, this information would often include attributes to 
facilitate the state machine operation like an identification number and a 
reference to the container. It is recommended to define this type in the 
PulseCounterMgr implementation file.
\snippet doc/examples/PulseCounterMgr/PulseCounterMgr.c PulseCounter definition 

\subsubsection qref19_1_2 Define the type of the container active object 
This type is derived from RKH_SMA_T type, so an attribute of RKH_SMA_T type has 
to be defined as its first attribute. The active object maintains an instance 
of the parameterized state machine, which is defined as an array of state 
machines. If this type is defined in the PulseCounterMgr implementation file, 
it will be an opaque type.
\snippet doc/examples/PulseCounterMgr/PulseCounterMgr.c PulseCounterMgr definition 

\subsubsection qref19_1_3 Declare the active object type 
\snippet doc/examples/PulseCounterMgr/PulseCounterMgr.h Type declaration

\subsubsection qref19_1_4 Instantiate the active object
\snippet doc/examples/PulseCounterMgr/PulseCounterMgr.c PulseCounterMgr instance

\subsubsection qref19_1_5 Define and declare a reference to the active object
\snippet doc/examples/PulseCounterMgr/PulseCounterMgr.c PulseCounterMgr pointer definition
\snippet doc/examples/PulseCounterMgr/PulseCounterMgr.h PulseCounterMgr pointer declaration

\subsubsection qref19_1_6 Instantiate the constant part of the parameterized state machine
\snippet doc/examples/PulseCounterMgr/PulseCounterMgr.c PulseCounter instance

\subsubsection qref19_1_7 Define and declare a constructor
The PulseCounterMgr constructor initializes the PulseCounterMgr's virtual 
table and its state machine components.
\snippet doc/examples/PulseCounterMgr/PulseCounterMgr.c Constructor

The type PulseCounterMgr specializes two functions of RKHSmaVtbl, 
RKHSmaVtbl::activate and RKHSmaVtbl::task. 
The first one initializes every state machine component by calling the 
framework function rkh_sm_init(). It effectively triggers the topmost 
initial transition of a state machine.
\snippet doc/examples/PulseCounterMgr/PulseCounterMgr.c Activate 

The specialized version of RKHSmaVtbl::task dispatches received events to 
the corresponding PulseCounter state machine.
\snippet doc/examples/PulseCounterMgr/PulseCounterMgr.c Task

\subsection qref19_2 Non-reactive active object
...

\subsection qref19_3 Periodic active object
...

\subsection qref19_3_1 Non-reactive

- \ref qref19_3_1_1
- \ref qref19_3_1_2

\subsubsection qref19_3_1_1 Simple
...

\subsubsection qref19_3_1_2 But using a state machine with synchronous inputs
...

\subsection qref19_3_2 Reactive
...

- \ref qref19_3_2_1
- \ref qref19_3_2_2

\subsubsection qref19_3_2_1 Using orthogonal regions
...

\subsubsection qref19_3_2_2 Propagating events
...

\subsection qref19_customization Customization

Each RKH application must have its own configuration file, called 
\c rkhcfg.h. This file adapts and configures RKH by means of compiler
definitions and macros allowing to restrict the resources consumed by RKH.
Adjusting this definitions allows to reduce the ROM and RAM consumption,
and to enhance the system performance in a substantial manner. The 
\c rkhcfg.h shows the general layout of the configuration file.
Use the following macros to reduce the memory taken by state machine 
structure: #RKH_CFG_TMR_EN, #RKH_CFG_TMR_SIZEOF_NTIMER, #RKH_CFG_TMR_HOOK_EN, 
#RKH_CFG_TMR_GET_INFO_EN.
See \ref cfg section for more information. 

Prev: \ref main_page "Home"

\page dbg Tracing tool
\image html rkh_bunner.jpg

Prev: \ref main_page "Home" \n
Next: \ref utest

\tableofcontents

When a program needs to be traced, it has to generate some information 
each time it reaches a "significant step" (certain instruction in the 
program's source code). In the standard terminology, this step is called 
a trace point, and the tracing information which is generated at that 
point is called a trace event. A program containing one or more of this 
trace points is named instrumented application.

The definition of events and the mapping between these and their 
corresponding names is hard-coded in the RKH implementation. Therefore, 
these events are common for all the state machine applications and never 
change (they are always traced). 
The trace events are associated with a integer value and are explicity 
listed and defined (enumerated) as shown below in this file.
	
The standard defines that the trace system has to store some information 
for each trace event (also named arguments) being generated, including, 
at least, the following:

- the trace event identifier (RKH_TE_<group>_<event> definitions),
- a timestamp (optional),
- any extra data that the system wants to associate with the event (optional).
 
When the system or an application trace an event, all the information 
related to it has to be stored somewhere before it can be retrieved, in 
order to be analyzed. This place is a trace stream. Formally speaking, a 
trace stream is defined as a non-persistent, internal (opaque) data 
object containing a sequence of trace events plus some internal 
information to interpret those trace events.

Also, the streams support runtime filtering. The application can define and 
apply a filter to a trace stream. Basically, the filter establishes which 
event types the stream is accepting (and hence storing) and which are not.
Therefore, trace events corresponding to types which are filtered out 
from a certain stream will not be stored in the stream. The stream in the 
system can potentially be applied a different filter. This filter can be 
applied, removed or changed at any time.

\n This section includes:

- \ref tre
- \ref trcfg
- \ref trfn
- \ref trfil
- \ref trtbl
- \ref trtrazer
- \ref trazertc

<HR>
\section tre Trace event structure
\copydetails rkhtrc.h

<HR>
\section trcfg Trace tool configuration

First of all, RKH has a set of configuration options related to trace tool 
facility, which an user that require this feature must be properly configure 
in the \c rkhcfg.h header file. See \ref cfg section for more information 
about that.

<HR>
\section trfn Implementing the trace session support

For using the native trace facility the user should implement several 
functions which are platform and application specific. These function 
prototypes are definied within \c rkh.h file and listed below:

\li \b rkh_trc_open() \copydetails RKH_TRC_OPEN

\li \b rkh_trc_close() \copydetails RKH_TRC_CLOSE

\li \b rkh_trc_flush() \copydetails RKH_TRC_FLUSH

\li \b rkh_trc_getts() \copydetails rkh_trc_getts

<HR>
\section trfil Using runtime trace filters

Also, the streams support runtime filtering. The application can define and 
apply a filter to a trace stream. Basically, the filter establishes which 
event types the stream is accepting (and hence storing) and which are not.
Therefore, trace events corresponding to types which are filtered out 
from the stream will not be stored in the stream. The stream in the 
system can potentially be applied a different filters. This filter can be 
applied, removed or changed at any time. A filter could be applied either 
specific trace event or all events from a specific group.

This section includes:

- \ref trfil_group
- \ref trfil_event
- \ref trfil_sma
- \ref trfil_sig

\subsection trfil_group Emit or suppress all trace events from a specific group

The stream is initially created with an empty filter (that is, without 
filtering any event type). If this is not the required behavior, the 
application can build a set of event types, include the appropriate event 
types in it, and apply it as a filter to the stream. After that, the 
stream will reject any event whose type is in the filter set.

Gathering many events generates a lot of data, which requires memory and 
processor time. It also makes the task of interpreting the data more 
difficult. Because the amount of data that the instrumented framework 
generates can be overwhelming, the RKH supports several types of filters 
that can use it to reduce the amount of data to be processed. The available 
groups are enumerated in RKH_TG_<group>.

Please use RKH_FILTER_ON_GROUP(), RKH_FILTER_ON_GROUP_ALL_EVENTS(), 
RKH_FILTER_OFF_GROUP_ALL_EVENTS(), or RKH_FILTER_OFF_GROUP() macros to do 
that.

Example:
 	
\code
RKH_FILTER_ON_GROUP( RKH_TRC_ALL_GROUPS );
RKH_FILTER_ON_EVENT( RKH_TRC_ALL_EVENTS );
RKH_FILTER_OFF_GROUP_ALL_EVENTS( RKH_TG_FWK );
RKH_FILTER_ON_GROUP_ALL_EVENTS( RKH_TG_SM );
\endcode

\subsection trfil_event Emit or suppress a specific event

The stream is initially created with an empty filter (that is, without 
filtering any event type). If this is not the required behavior, the 
application can build a set of event types, include the appropriate event 
types in it, and apply it as a filter to the stream. After that, the 
stream will reject any event whose type is in the filter set.

Gathering many events generates a lot of data, which requires memory and 
processor time. It also makes the task of interpreting the data more 
difficult. Because the amount of data that the instrumented framework 
generates can be overwhelming, the RKH supports several types of filters 
that can use it to reduce the amount of data to be processed. The available 
events are enumerated in RKH_TE_<group>_<event> definitions.

Please use RKH_FILTER_ON_EVENT(), or RKH_FILTER_OFF_EVENT() macros to do 
that.

Example:
 	
\code
RKH_FILTER_OFF_EVENT( RKH_TG_MP, RKH_TE_MP_INIT );
RKH_FILTER_OFF_EVENT( RKH_TG_TMR, RKH_TE_TMR_START );
RKH_FILTER_OFF_EVENT( RKH_TG_TMR, RKH_TE_TMR_TOUT );
\endcode

\subsection trfil_sma 	Emit or suppress the trace events related to a specific SMA (active object)

Please use RKH_FILTER_ON_SMA(), RKH_FILTER_OFF_SMA(), 
RKH_FILTER_ON_ALL_SMA()/RKH_FILTER_OFF_ALL_SMA to do that.

Example:
	
\code
RKH_FILTER_OFF_SMA( blinky );
\endcode

\subsection trfil_sig 	Emit or suppress the trace events related to a specific signal (user event).

Please use RKH_FILTER_ON_SIGNAL(), RKH_FILTER_OFF_SIGNAL(), 
RKH_FILTER_ON_ALL_SIGNALS()/RKH_FILTER_OFF_ALL_SIGNALS() to do that.

Example:
	
\code
RKH_FILTER_OFF_SIGNAL( REQ );
RKH_FILTER_OFF_SIGNAL( START );
\endcode

<HR>
\section trtbl Trace event table

This section provides a table that lists all the trace events and summarizes 
the data included for each. 

<TABLE	align="center" valign="middle" width=95% cellspacing=2 
		cellpadding=4 border=0>
	<TR bgColor="#c0c0c0">
		<TH colspan=6 align="left"><B> Related with memory pool 
											module (MP)</B></TH>
	</TR>
	<TR bgColor="#c0c0c0">
		<TH><B><I> Group </I></B></TH>
		<TH><B><I> ID </I></B></TH>
		<TH width=35%><B><I> Trace Event </I></B></TH> 
		<TH width=26%><B><I> Description </I></B></TH> 
		<TH colspan=2><B><I> Parameters </I></B></TH> 
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=8 align="center"> #RKH_TG_MP </TD>
		<TD rowspan=3 align="center"> 0 </TD>
		<TD rowspan=3> #RKH_TE_MP_INIT(SYM mp, NBLK nblock, 
														BSIZE bsize) </TD>
		<TD rowspan=3> \copybrief RKH_TR_MP_INIT </TD>
		<TD><I> mp </I></TD>
		<TD><I> Pointer to previously allocated memory pool structure </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> nblock </I></TD>
		<TD><I> \copybrief RKH_MEMPOOL_T::nblocks </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> bsize </I></TD>
		<TD><I> \copybrief RKH_MEMPOOL_T::bsize </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=3 align="center"> 1 </TD>
		<TD rowspan=3> #RKH_TE_MP_GET (SYM mp, NBLK nfree, NBLK nmin) </TD>
		<TD rowspan=3> \copybrief RKH_TR_MP_GET </TD>
		<TD><I> mp </I></TD>
		<TD><I> Pointer to previously allocated memory pool structure </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> nfree </I></TD>
		<TD><I> \copybrief RKH_MEMPOOL_T::nfree </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> nmin </I></TD>
		<TD><I> \copybrief RKH_MEMPOOL_T::nmin </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=2 align="center"> 2 </TD>
		<TD rowspan=2> #RKH_TE_MP_PUT (SYM mp, NBLK nfree) </TD>
		<TD rowspan=2> \copybrief RKH_TR_MP_PUT </TD>
		<TD><I> mp </I></TD>
		<TD><I> Pointer to previously allocated memory pool structure </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> nfree </I></TD>
		<TD><I> \copybrief RKH_MEMPOOL_T::nfree </I></TD>
	</TR>

	<TR bgColor="#c0c0c0">
		<TH colspan=6 align="left"><B> Related with queue module (QUE)</B></TH>
	</TR>
	<TR bgColor="#c0c0c0">
		<TH><B><I> Group </I></B></TH>
		<TH><B><I> ID </I></B></TH>
		<TH><B><I> Trace Event </I></B></TH> 
		<TH><B><I> Description </I></B></TH> 
		<TH colspan=2><B><I> Parameters </I></B></TH> 
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=14 align="center"> #RKH_TG_QUE </TD>
		<TD rowspan=3 align="center"> 0 </TD>
		<TD rowspan=3> #RKH_TE_QUE_INIT (SYM q, SYM ao, NE nelem) </TD>
		<TD rowspan=3> \copybrief RKH_TR_QUE_INIT </TD>
		<TD><I> q </I></TD>
		<TD><I> \copybrief RKH_SMA_T::equeue </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> ao </I></TD>
		<TD><I> \copybrief RKH_QUEUE_T::sma </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> nelem </I></TD>
		<TD><I> \copybrief RKH_QUEUE_T::nelems </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=2 align="center"> 1 </TD>
		<TD rowspan=2> #RKH_TE_QUE_GET (SYM q, NE nelem) </TD>
		<TD rowspan=2> \copybrief RKH_TR_QUE_GET </TD>
		<TD><I> q </I></TD>
		<TD><I> \copybrief RKH_SMA_T::equeue </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> nelem </I></TD>
		<TD><I> \copybrief RKH_QUEUE_T::nelems </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=3 align="center"> 2 </TD>
		<TD rowspan=3> #RKH_TE_QUE_FIFO (SYM q, NE nelem, NE nmin) </TD>
		<TD rowspan=3> \copybrief RKH_TR_QUE_FIFO </TD>
		<TD><I> q </I></TD>
		<TD><I> \copybrief RKH_SMA_T::equeue </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> nelem </I></TD>
		<TD><I> \copybrief RKH_QUEUE_T::nelems </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> nmin </I></TD>
		<TD><I> \copybrief RKH_QUEUE_T::nmin </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=3 align="center"> 3 </TD>
		<TD rowspan=3> #RKH_TE_QUE_LIFO (SYM q, NE nelem, NE nmin) </TD>
		<TD rowspan=3> \copybrief RKH_TR_QUE_LIFO </TD>
		<TD><I> q </I></TD>
		<TD><I> \copybrief RKH_SMA_T::equeue </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> nelem </I></TD>
		<TD><I> \copybrief RKH_QUEUE_T::nelems </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> nmin </I></TD>
		<TD><I> \copybrief RKH_QUEUE_T::nmin </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=1 align="center"> 4 </TD>
		<TD rowspan=1> #RKH_TE_QUE_FULL (SYM q) </TD>
		<TD rowspan=1> \copybrief RKH_TR_QUE_FULL </TD>
		<TD><I> q </I></TD>
		<TD><I> \copybrief RKH_SMA_T::equeue </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=1 align="center"> 5 </TD>
		<TD rowspan=1> #RKH_TE_QUE_DPT (SYM q) </TD>
		<TD rowspan=1> \copybrief RKH_TR_QUE_DPT </TD>
		<TD><I> q </I></TD>
		<TD><I> \copybrief RKH_SMA_T::equeue </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=1 align="center"> 6 </TD>
		<TD rowspan=1> #RKH_TE_QUE_GET_LAST (SYM q) </TD>
		<TD rowspan=1> \copybrief RKH_TR_QUE_GET_LAST </TD>
		<TD><I> q </I></TD>
		<TD><I> \copybrief RKH_SMA_T::equeue </I></TD>
	</TR>

	<TR bgColor="#c0c0c0">
		<TH colspan=6 align="left"><B> Related with State Machine Application 
											(SMA) or Active Object </B></TH>
	</TR>
	<TR bgColor="#c0c0c0">
		<TH><B><I> Group </I></B></TH>
		<TH><B><I> ID </I></B></TH>
		<TH><B><I> Trace Event </I></B></TH> 
		<TH><B><I> Description </I></B></TH> 
		<TH colspan=2><B><I> Parameters </I></B></TH> 
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=33 align="center"> #RKH_TG_SMA </TD>
		<TD rowspan=3 align="center"> 0 </TD>
		<TD rowspan=3> #RKH_TE_SMA_ACT (SYM ao, UI8 prio, NE msgQueueSize) </TD>
		<TD rowspan=3> \copybrief RKH_TR_SMA_ACT </TD>
		<TD><I> ao </I></TD>
		<TD><I> \copybrief RKH_QUEUE_T::sma </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> prio </I></TD>
		<TD><I> \copybrief RKH_ROM_T::prio </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> msgQueueSize </I></TD>
		<TD><I> \copybrief RKH_QUEUE_T::nelems </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=2 align="center"> 1 </TD>
		<TD rowspan=2> #RKH_TE_SMA_TERM (SYM ao, UI8 prio) </TD>
		<TD rowspan=2> \copybrief RKH_TR_SMA_TERM </TD>
		<TD><I> ao </I></TD>
		<TD><I> \copybrief RKH_QUEUE_T::sma </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> prio </I></TD>
		<TD><I> \copybrief RKH_ROM_T::prio </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=6 align="center"> 2 </TD>
		<TD rowspan=6> #RKH_TE_SMA_GET (SYM ao, SIG sig, UI8 pid, 
										UI8 refc, NE nElem, NE nMin) </TD>
		<TD rowspan=6> \copybrief RKH_TR_SMA_GET </TD>
		<TD><I> ao </I></TD>
		<TD><I> \copybrief RKH_QUEUE_T::sma </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> sig </I></TD>
		<TD><I> \copybrief RKH_EVT_T::e </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> pid </I></TD>
		<TD><I> \copybrief RKH_EVT_T::nref </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> refc </I></TD>
		<TD><I> \copybrief RKH_EVT_T::nref </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> nElem </I></TD>
		<TD><I> \copybrief RKH_QUEUE_T::qty </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> nMin </I></TD>
		<TD><I> \copybrief RKH_QUEUE_T::nmin </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=7 align="center"> 3 </TD>
		<TD rowspan=7> #RKH_TE_SMA_FIFO(SYM ao, SIG sig, SYM snr,
									    UI8 pid, UI8 refc, 
                                        NE nElem, Ne nMin) </TD>
		<TD rowspan=7> \copybrief RKH_TR_SMA_FIFO </TD>
		<TD><I> ao </I></TD>
		<TD><I> \copybrief RKH_QUEUE_T::sma </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> sig </I></TD>
		<TD><I> \copybrief RKH_EVT_T::e </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> snr </I></TD>
		<TD><I> Sender object </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> pid </I></TD>
		<TD><I> \copybrief RKH_EVT_T::nref </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> refc </I></TD>
		<TD><I> \copybrief RKH_EVT_T::nref </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> nElem </I></TD>
		<TD><I> \copybrief RKH_QUEUE_T::qty </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> nMin </I></TD>
		<TD><I> \copybrief RKH_QUEUE_T::nmin </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=7 align="center"> 4 </TD>
		<TD rowspan=7> #RKH_TE_SMA_LIFO(SYM ao, SIG sig, SYM snr, 
										UI8 pid, UI8 refc,
                                        NE nElem, NE nMin) </TD>
		<TD rowspan=7> \copybrief RKH_TR_SMA_LIFO </TD>
		<TD><I> ao </I></TD>
		<TD><I> \copybrief RKH_QUEUE_T::sma </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> sig </I></TD>
		<TD><I> \copybrief RKH_EVT_T::e </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> snr </I></TD>
		<TD><I> Sender object </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> pid </I></TD>
		<TD><I> \copybrief RKH_EVT_T::nref </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> refc </I></TD>
		<TD><I> \copybrief RKH_EVT_T::nref </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> nElem </I></TD>
		<TD><I> \copybrief RKH_QUEUE_T::qty </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> nMin </I></TD>
		<TD><I> \copybrief RKH_QUEUE_T::nmin </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=2 align="center"> 5 </TD>
		<TD rowspan=2> #RKH_TE_SMA_REG (SYM ao, UI8 prio) </TD>
		<TD rowspan=2> \copybrief RKH_TR_SMA_REG </TD>
		<TD><I> ao </I></TD>
		<TD><I> \copybrief RKH_QUEUE_T::sma </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> prio </I></TD>
		<TD><I> \copybrief RKH_ROM_T::prio </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=2 align="center"> 6 </TD>
		<TD rowspan=2> #RKH_TE_SMA_UNREG (SYM ao, UI8 prio) </TD>
		<TD rowspan=2> \copybrief RKH_TR_SMA_UNREG </TD>
		<TD><I> ao </I></TD>
		<TD><I> \copybrief RKH_QUEUE_T::sma </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> prio </I></TD>
		<TD><I> \copybrief RKH_ROM_T::prio </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=2 align="center"> 6 </TD>
		<TD rowspan=2> #RKH_TE_SMA_DEFER (SYM q, SIG sig) </TD>
		<TD rowspan=2> \copybrief RKH_TR_SMA_DEFER </TD>
		<TD><I> q </I></TD>
		<TD><I> \copybrief RKH_SMA_T::equeue </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> sig </I></TD>
		<TD><I> \copybrief RKH_EVT_T::e </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=2 align="center"> 7 </TD>
		<TD rowspan=2> #RKH_TE_SMA_RCALL (SYM ao, SIG sig) </TD>
		<TD rowspan=2> \copybrief RKH_TR_SMA_RCALL </TD>
		<TD><I> ao </I></TD>
		<TD><I> \copybrief RKH_QUEUE_T::sma </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> sig </I></TD>
		<TD><I> \copybrief RKH_EVT_T::e </I></TD>
	</TR>

	<TR bgColor="#c0c0c0">
		<TH colspan=6 align="left"><B> Related with State Machines (SM)</B></TH>
	</TR>
	<TR bgColor="#c0c0c0">
		<TH><B><I> Group </I></B></TH>
		<TH><B><I> ID </I></B></TH>
		<TH><B><I> Trace Event </I></B></TH> 
		<TH><B><I> Description </I></B></TH> 
		<TH colspan=2><B><I> Parameters </I></B></TH> 
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=36 align="center"> #RKH_TG_SM </TD>
		<TD rowspan=2 align="center"> 0 </TD>
		<TD rowspan=2> #RKH_TE_SM_INIT (SYM ao, SYM ist) </TD>
		<TD rowspan=2> \copybrief RKH_TR_SM_INIT </TD>
		<TD><I> ao </I></TD>
		<TD><I> \copybrief RKH_QUEUE_T::sma </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> ist </I></TD>
		<TD><I> \copybrief RKH_ROM_T::istate </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=2 align="center"> 1 </TD>
		<TD rowspan=2> #RKH_TE_SM_CLRH (SYM ao, SYM h) </TD>
		<TD rowspan=2> \copybrief RKH_TR_SM_CLRH </TD>
		<TD><I> ao </I></TD>
		<TD><I> \copybrief RKH_QUEUE_T::sma </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> h </I></TD>
		<TD><I> \copybrief RKH_SCMP_T::history </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=3 align="center"> 3 </TD>
		<TD rowspan=3> #RKH_TE_SM_TRN (SYM ao, SYM sst, SYM tst) </TD>
		<TD rowspan=3> \copybrief RKH_TR_SM_TRN </TD>
		<TD><I> ao </I></TD>
		<TD><I> \copybrief RKH_QUEUE_T::sma </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> sst </I></TD>
		<TD><I> Source state of transition </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> tst </I></TD>
		<TD><I> Target state of transition </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=2 align="center"> 4 </TD>
		<TD rowspan=2> #RKH_TE_SM_STATE (SYM ao, SYM st) </TD>
		<TD rowspan=2> \copybrief RKH_TR_SM_STATE </TD>
		<TD><I> ao </I></TD>
		<TD><I> \copybrief RKH_QUEUE_T::sma </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> nxtst </I></TD>
		<TD><I> Next state of transition </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=2 align="center"> 5 </TD>
		<TD rowspan=2> #RKH_TE_SM_ENSTATE (SYM ao, SYM st) </TD>
		<TD rowspan=2> \copybrief RKH_TR_SM_ENSTATE </TD>
		<TD><I> ao </I></TD>
		<TD><I> \copybrief RKH_QUEUE_T::sma </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> st </I></TD>
		<TD><I> Entry state </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=2 align="center"> 6 </TD>
		<TD rowspan=2> #RKH_TE_SM_EXSTATE (SYM ao, SYM st) </TD>
		<TD rowspan=2> \copybrief RKH_TR_SM_EXSTATE </TD>
		<TD><I> ao </I></TD>
		<TD><I> \copybrief RKH_QUEUE_T::sma </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> st </I></TD>
		<TD><I> Exited state </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=3 align="center"> 7 </TD>
		<TD rowspan=3> #RKH_TE_SM_NENEX (SYM ao, UI8 nen, UI8 nex) </TD>
		<TD rowspan=3> \copybrief RKH_TR_SM_NENEX </TD>
		<TD><I> ao </I></TD>
		<TD><I> \copybrief RKH_QUEUE_T::sma </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> nen </I></TD>
		<TD><I> Number of entry states </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> nex </I></TD>
		<TD><I> Number of exited states </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=3 align="center"> 8 </TD>
		<TD rowspan=3> #RKH_TE_SM_NTRNACT (SYM ao, UI8 nta, UI8 nts) </TD>
		<TD rowspan=3> \copybrief RKH_TR_SM_NTRNACT </TD>
		<TD><I> ao </I></TD>
		<TD><I> \copybrief RKH_QUEUE_T::sma </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> nta </I></TD>
		<TD><I> Number of executed actions </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> nts </I></TD>
		<TD><I> Number of transition segments </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=2 align="center"> 9 </TD>
		<TD rowspan=2> #RKH_TE_SM_TS_STATE (SYM ao, SYM st) </TD>
		<TD rowspan=2> \copybrief RKH_TR_SM_TS_STATE </TD>
		<TD><I> ao </I></TD>
		<TD><I> \copybrief RKH_QUEUE_T::sma </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> st </I></TD>
		<TD><I> Next state or pseudostate in transition </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=1 align="center"> 10 </TD>
		<TD rowspan=1> #RKH_TE_SM_EVT_PROC (SYM ao) </TD>
		<TD rowspan=1> \copybrief RKH_TR_SM_EVT_PROC </TD>
		<TD><I> ao </I></TD>
		<TD><I> \copybrief RKH_QUEUE_T::sma </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=2 align="center"> 2 </TD>
		<TD rowspan=2> #RKH_TE_SM_EVT_NFOUND (SYM ao, SIG sig) </TD>
		<TD rowspan=2> \copybrief RKH_TR_SM_EVT_NFOUND </TD>
		<TD><I> ao </I></TD>
		<TD><I> \copybrief RKH_QUEUE_T::sma </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> sig </I></TD>
		<TD><I> \copybrief RKH_EVT_T::e </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=1 align="center"> 12 </TD>
		<TD rowspan=1> #RKH_TE_SM_CND_NFOUND (SYM ao) </TD>
		<TD rowspan=1> \copybrief RKH_TR_SM_CND_NFOUND </TD>
		<TD><I> ao </I></TD>
		<TD><I> \copybrief RKH_QUEUE_T::sma </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=1 align="center"> 13 </TD>
		<TD rowspan=1> #RKH_TE_SM_GRD_FALSE (SYM ao) </TD>
		<TD rowspan=1> \copybrief RKH_TR_SM_GRD_FALSE </TD>
		<TD><I> ao </I></TD>
		<TD><I> \copybrief RKH_QUEUE_T::sma </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=1 align="center"> 14 </TD>
		<TD rowspan=1> #RKH_TE_SM_UNKN_STATE (SYM ao) </TD>
		<TD rowspan=1> \copybrief RKH_TR_SM_UNKN_STATE </TD>
		<TD><I> ao </I></TD>
		<TD><I> \copybrief RKH_QUEUE_T::sma </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=1 align="center"> 15 </TD>
		<TD rowspan=1> #RKH_TE_SM_EX_HLEVEL (SYM ao) </TD>
		<TD rowspan=1> \copybrief RKH_TR_SM_EX_HLEVEL </TD>
		<TD><I> ao </I></TD>
		<TD><I> \copybrief RKH_QUEUE_T::sma </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=1 align="center"> 16 </TD>
		<TD rowspan=1> #RKH_TE_SM_EX_TSEG (SYM ao) </TD>
		<TD rowspan=1> \copybrief RKH_TR_SM_EX_TSEG </TD>
		<TD><I> ao </I></TD>
		<TD><I> \copybrief RKH_QUEUE_T::sma </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=4 align="center"> 8 </TD>
		<TD rowspan=4> #RKH_TE_SM_EXE_ACT (UI8 act_type, SYM ao, SYM st, 
                                           FUN act)</TD>
		<TD rowspan=4> \copybrief RKH_TR_SM_EXE_ACT </TD>
		<TD><I> act_type </I></TD>
		<TD><I> \copybrief RKH_SUBTE_SM_EXE_ACT </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> ao </I></TD>
		<TD><I> \copybrief RKH_QUEUE_T::sma </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> st </I></TD>
		<TD><I> Points to associated state </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> act </I></TD>
		<TD><I> Points to executed action </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=3 align="center"> 2 </TD>
		<TD rowspan=3> #RKH_TE_SM_DCH (SYM ao, SIG sig, SYM st) </TD>
		<TD rowspan=3> \copybrief RKH_TR_SM_DCH </TD>
		<TD><I> ao </I></TD>
		<TD><I> \copybrief RKH_QUEUE_T::sma </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> sig </I></TD>
		<TD><I> \copybrief RKH_EVT_T::e </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> st </I></TD>
		<TD><I> Current state (basic or substate) </I></TD>
	</TR>

	<TR bgColor="#c0c0c0">
		<TH colspan=6 align="left"><B> Related with timer module (TMR)</B></TH>
	</TR>
	<TR bgColor="#c0c0c0">
		<TH><B><I> Group </I></B></TH>
		<TH><B><I> ID </I></B></TH>
		<TH><B><I> Trace Event </I></B></TH> 
		<TH><B><I> Description </I></B></TH> 
		<TH colspan=2><B><I> Parameters </I></B></TH> 
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=13 align="center"> #RKH_TG_TMR </TD>
		<TD rowspan=2 align="center"> 0 </TD>
		<TD rowspan=2> #RKH_TE_TMR_INIT (SYM t, SIG sig) </TD>
		<TD rowspan=2> \copybrief RKH_TR_TMR_INIT </TD>
		<TD><I> t </I></TD>
		<TD><I> Pointer to previously allocated timer structure </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> sig </I></TD>
		<TD><I> \copybrief RKH_EVT_T::e </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=4 align="center"> 1 </TD>
		<TD rowspan=4> #RKH_TE_TMR_START (SYM t, SYM ao, NTICK ntick, 
															NTICK per) </TD>
		<TD rowspan=4> \copybrief RKH_TR_TMR_START </TD>
		<TD><I> t </I></TD>
		<TD><I> Pointer to previously allocated timer structure </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> ao </I></TD>
		<TD><I> \copybrief RKH_TMR_T::sma </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> ntick </I></TD>
		<TD><I> \copybrief RKH_TMR_T::ntick </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> per </I></TD>
		<TD><I> \copybrief RKH_TMR_T::period </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=3 align="center"> 2 </TD>
		<TD rowspan=3> #RKH_TE_TMR_STOP (SYM t, NTICK ntick, NTICK per) </TD>
		<TD rowspan=3> \copybrief RKH_TR_TMR_STOP </TD>
		<TD><I> t </I></TD>
		<TD><I> Pointer to previously allocated timer structure </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> ntick </I></TD>
		<TD><I> \copybrief RKH_TMR_T::ntick </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> per </I></TD>
		<TD><I> \copybrief RKH_TMR_T::period </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=3 align="center"> 3 </TD>
		<TD rowspan=3> #RKH_TE_TMR_TOUT (SYM t, SIG sig, SYM ao) </TD>
		<TD rowspan=3> \copybrief RKH_TR_TMR_TOUT </TD>
		<TD><I> t </I></TD>
		<TD><I> Pointer to previously allocated timer structure </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> sig </I></TD>
		<TD><I> \copybrief RKH_EVT_T::e </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> ao </I></TD>
		<TD><I> \copybrief RKH_TMR_T::sma </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=1 align="center"> 4 </TD>
		<TD rowspan=1> #RKH_TE_TMR_REM (SYM t) </TD>
		<TD rowspan=1> \copybrief RKH_TR_TMR_REM </TD>
		<TD><I> t </I></TD>
		<TD><I> Pointer to previously allocated timer structure </I></TD>
	</TR>

	<TR bgColor="#c0c0c0">
		<TH colspan=6 align="left"><B> Miscellanueos and related with 
										Framework (FWK)</B></TH>
	</TR>
	<TR bgColor="#c0c0c0">
		<TH><B><I> Group </I></B></TH>
		<TH><B><I> ID </I></B></TH>
		<TH><B><I> Trace Event </I></B></TH> 
		<TH><B><I> Description </I></B></TH> 
		<TH colspan=2><B><I> Parameters </I></B></TH> 
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=55 align="center"> #RKH_TG_FWK </TD>
		<TD rowspan=1 align="center"> 0 </TD>
		<TD rowspan=1> #RKH_TE_FWK_EN () </TD>
		<TD rowspan=1> \copybrief RKH_TR_FWK_EN </TD>
		<TD><I></I></TD>
		<TD><I></I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=1 align="center"> 1 </TD>
		<TD rowspan=1> #RKH_TE_FWK_EX () </TD>
		<TD rowspan=1> \copybrief RKH_TR_FWK_EX </TD>
		<TD><I></I></TD>
		<TD><I></I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=3 align="center"> 2 </TD>
		<TD rowspan=3> #RKH_TE_FWK_EPREG (UI8 ep, UI32 ss, ES es) </TD>
		<TD rowspan=3> \copybrief RKH_TR_FWK_EPREG </TD>
		<TD><I> ep </I></TD>
		<TD><I> Event pool index in the list </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> ss </I></TD>
		<TD><I> Storage size in bytes </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> es </I></TD>
		<TD><I> Event size </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=7 align="center"> 3 </TD>
		<TD rowspan=7> #RKH_TE_FWK_AE (ES es, SIG sig, UI8 pid, 
															UI8 refc) </TD>
		<TD rowspan=7> \copybrief RKH_TR_FWK_AE </TD>
		<TD><I> es </I></TD>
		<TD><I> Event size </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> sig </I></TD>
		<TD><I> \copybrief RKH_EVT_T::e </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> pid </I></TD>
		<TD><I> \copybrief RKH_EVT_T::nref </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> refc </I></TD>
		<TD><I> \copybrief RKH_EVT_T::nref </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> nUsed </I></TD>
		<TD><I> Current number of memory blocks used </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> nMin </I></TD>
		<TD><I> \copybrief RKH_MEMPOOL_T::nmin </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> sender </I></TD>
		<TD><I> Pointer to the actor that request a memory block </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=3 align="center"> 4 </TD>
		<TD rowspan=3> #RKH_TE_FWK_GC (SIG sig, UI8 pid, UI8 refc) </TD>
		<TD rowspan=3> \copybrief RKH_TR_FWK_GC </TD>
		<TD><I> sig </I></TD>
		<TD><I> \copybrief RKH_EVT_T::e </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> pid </I></TD>
		<TD><I> \copybrief RKH_EVT_T::nref </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> refc </I></TD>
		<TD><I> \copybrief RKH_EVT_T::nref </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=6 align="center"> 5 </TD>
		<TD rowspan=6> #RKH_TE_FWK_GCR (SIG sig, UI8 pid, UI8 refc) </TD>
		<TD rowspan=6> \copybrief RKH_TR_FWK_GCR </TD>
		<TD><I> sig </I></TD>
		<TD><I> \copybrief RKH_EVT_T::e </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> pid </I></TD>
		<TD><I> \copybrief RKH_EVT_T::nref </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> refc </I></TD>
		<TD><I> \copybrief RKH_EVT_T::nref </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> nUsed </I></TD>
		<TD><I> Current number of memory blocks used </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> nMin </I></TD>
		<TD><I> \copybrief RKH_MEMPOOL_T::nmin </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> sender </I></TD>
		<TD><I> Pointer to the actor that request a memory block </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=3 align="center"> 4 </TD>
		<TD rowspan=3> #RKH_TE_FWK_GC (SIG sig, UI8 pid, UI8 refc) </TD>
		<TD rowspan=3> \copybrief RKH_TR_FWK_GC </TD>
		<TD><I> sig </I></TD>
		<TD><I> \copybrief RKH_EVT_T::e </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> pid </I></TD>
		<TD><I> \copybrief RKH_EVT_T::nref </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> refc </I></TD>
		<TD><I> \copybrief RKH_EVT_T::nref </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=2 align="center"> 8 </TD>
		<TD rowspan=2> #RKH_TE_FWK_OBJ (SYM obj, STR nm) </TD>
		<TD rowspan=2> \copybrief RKH_TR_FWK_OBJ </TD>
		<TD><I> obj </I></TD>
		<TD><I> Object memory address </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> nm </I></TD>
		<TD><I> Name of object </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=2 align="center"> 9 </TD>
		<TD rowspan=2> #RKH_TE_FWK_SIG (SIG sig, STR nm) </TD>
		<TD rowspan=2> \copybrief RKH_TR_FWK_SIG </TD>
		<TD><I> sig </I></TD>
		<TD><I> \copybrief RKH_EVT_T::e </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> nm </I></TD>
		<TD><I> Name of event signal </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=2 align="center"> 10 </TD>
		<TD rowspan=2> #RKH_TE_FWK_FUN (FUN func, STR nm) </TD>
		<TD rowspan=2> \copybrief RKH_TR_FWK_FUN </TD>
		<TD><I> func </I></TD>
		<TD><I> Function memory address </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> nm </I></TD>
		<TD><I> Name of object </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=1 align="center"> 11 </TD>
		<TD rowspan=1> #RKH_TE_FWK_EXE_FUN (FUN func) </TD>
		<TD rowspan=1> \copybrief RKH_TR_FWK_EXE_FUN </TD>
		<TD><I> func </I></TD>
		<TD><I> Function memory address </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=3 align="center"> 12 </TD>
		<TD rowspan=3> #RKH_TE_FWK_SYNC_EVT (FUN fn, SYM snr, SYM rcr) </TD>
		<TD rowspan=3> \copybrief RKH_TR_FWK_SYNC_EVT </TD>
		<TD><I> fn </I></TD>
		<TD><I> Function address </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> snr </I></TD>
		<TD><I> Sender object </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> rcr </I></TD>
		<TD><I> Receiver object </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=2 align="center"> 13 </TD>
		<TD rowspan=2> #RKH_TE_FWK_TUSR (UI8 usrtrc, STR nm) </TD>
		<TD rowspan=2> \copybrief RKH_TR_FWK_TUSR </TD>
		<TD><I> usrtrc </I></TD>
		<TD><I> User-defined trace event </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> nm </I></TD>
		<TD><I> Name of user-defined event </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=1 align="center"> 14 </TD>
		<TD rowspan=1> #RKH_TE_FWK_TCFG (CFG cfg) </TD>
		<TD rowspan=1> \copybrief RKH_TR_FWK_TCFG </TD>
		<TD><I> cfg </I></TD>
		<TD><I> Configuration parameters of trace facility </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=2 align="center"> 15 </TD>
		<TD rowspan=2> #RKH_TE_FWK_ASSERT (SRT mod, UI16 ln) </TD>
		<TD rowspan=2> \copybrief RKH_TR_FWK_ASSERT </TD>
		<TD><I> mod </I></TD>
		<TD><I> C/C++ module (.c file) </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> ln </I></TD>
		<TD><I> Line number of assertion </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=2 align="center"> 16 </TD>
		<TD rowspan=2> #RKH_TE_FWK_AO (SYM obj, STR nm) </TD>
		<TD rowspan=2> \copybrief RKH_TR_FWK_AO </TD>
		<TD><I> obj </I></TD>
		<TD><I> Active object memory address </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> nm </I></TD>
		<TD><I> Name of object </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=3 align="center"> 17 </TD>
		<TD rowspan=3> #RKH_TE_FWK_STATE (SYM ao, SYM obj, STR nm) </TD>
		<TD rowspan=3> \copybrief RKH_TR_FWK_STATE </TD>
		<TD><I> ao </I></TD>
		<TD><I> Active object memory address </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> obj </I></TD>
		<TD><I> State object memory address </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> nm </I></TD>
		<TD><I> Name of object </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=3 align="center"> 18 </TD>
		<TD rowspan=3> #RKH_TE_FWK_PSTATE (SYM ao, SYM obj, STR nm) </TD>
		<TD rowspan=3> \copybrief RKH_TR_FWK_PSTATE </TD>
		<TD><I> ao </I></TD>
		<TD><I> Active object memory address </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> obj </I></TD>
		<TD><I> Pseudostate object memory address </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> nm </I></TD>
		<TD><I> Name of object </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=2 align="center"> 19 </TD>
		<TD rowspan=2> #RKH_TE_FWK_TIMER (SYM obj, STR nm) </TD>
		<TD rowspan=2> \copybrief RKH_TR_FWK_TIMER </TD>
		<TD><I> obj </I></TD>
		<TD><I> Timer object memory address </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> nm </I></TD>
		<TD><I> Name of object </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=2 align="center"> 20 </TD>
		<TD rowspan=2> #RKH_TE_FWK_EPOOL (UI8 poolId, STR nm) </TD>
		<TD rowspan=2> \copybrief RKH_TR_FWK_EPOOL </TD>
		<TD><I> poolId </I></TD>
		<TD><I> Event pool ID </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> nm </I></TD>
		<TD><I> Name of object </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=2 align="center"> 21 </TD>
		<TD rowspan=2> #RKH_TE_FWK_QUEUE (SYM obj, STR nm) </TD>
		<TD rowspan=2> \copybrief RKH_TR_FWK_QUEUE </TD>
		<TD><I> obj </I></TD>
		<TD><I> Queue object memory address </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> nm </I></TD>
		<TD><I> Name of object </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD rowspan=2 align="center"> 22 </TD>
		<TD rowspan=2> #RKH_TE_FWK_ACTOR (SYM obj, STR nm) </TD>
		<TD rowspan=2> \copybrief RKH_TR_FWK_ACTOR </TD>
		<TD><I> obj </I></TD>
		<TD><I> Actor object address </I></TD>
	</TR>
	<TR bgColor="#f0f0f0" align="left" valign="middle" >
		<TD><I> nm </I></TD>
		<TD><I> Name of object </I></TD>
	</TR>
</TABLE>
\n

<HR>
\section trtrazer TRAZER - The fundamental RKH's tracing tool

Trazer is a visualization tool that works in conjuntion with the RKH 
framework built in trace facility. Trazer gives the possibility to display 
selectively the recording of all events of your system, state machines, 
queues, timers, etc. Trazer helps you to faster troubleshooting especially on 
complex problems where a debugger is not sufficient, by providing a simple 
consolidated, human-readable textual output. Given the RKH cross plataform 
portability, trace data may come from 8, 16, or 32-bits platforms. In order 
to that Trazer need to be configured to support this diversity of plataform 
and the wide range of RKH framework configurations. 

Here is the \ref trazer

<HR>
\section trazertc Reading and analysing traces using Trace Compass

In order to solve performance and reliability issues by reading and analyzing 
traces of a reactive and embedded application based on the RKH framework is 
proposed using the powerful Eclipse 
[Trace Compass](https://www.eclipse.org/tracecompass/).

Trace Compass is an open source application for viewing and analyzing any 
type of logs or traces. Its goal is to provide views, graphs, metrics, etc. 
to help extract useful information from traces, in a way that is more 
user-friendly and informative than huge text dumps.

\image html tc.png

The project [TraceTC](https://github.com/vortexmakes/TrazerTC) brings a 
simple guide on how to use Trace Compass together with RKH tracing tool.

Prev: \ref main_page "Home" \n
Next: \ref utest

\page cross Cross-platform demos
\image html rkh_bunner.jpg

Prev: \ref main_page "Home" \n
Next: \ref Download "Download"

\tableofcontents

This section provides several examples on how to use RKH, designed to 
experiment with RKH immediately. The simplicity of the code and state 
diagrams means it also serves as a good starter project for people who are 
not yet familiar with RKH. Also, this examples are demostration projects 
considered cross-platform, because of they are able to function on more 
than one computer architecture or operating system. 
The code of demos are located in the \c \<rkh\>/demo/cross/ directory, 
where \c \<rkh\> stands for the installation directory chosed to install 
the accompanying software. Each example contains the platform-independent 
source code and a \c build/ directory. The structure of the 
\c build branch closely mirrors the structure of the 
\c source/portable/ directory, which contains platform-specific header 
files and libraries to be used by RKH applications.


\image html rkh_cross.png "RKH cross-platform architecture"

- (1)	Reactive applications. The user code, which defines the active 
		object, state machines, events, and the RKH configurations.
- (2)	RKH framework. Platform-neutral code.
- (3)	Platform abstraction layer, which encapsulates all the 
		platform-specific code (processor, compiler, and operating system) 
		and cleanly separates it from the platform-neutral code, (1) and (2) 
		layers.
- (4)	Board support package (BSP). Platform-dependent files, which 
		implements RKH support on each platform defined by the (3) layer.
- (5)	RKH port. Implements all the platform-specific code, which is defined 
		by processor, hardware, compiler, and operating system in use.
- (6)	Target hardware.

\n RKH cross-platform examples:

- \subpage cross_blinky
- \subpage cross_shared

Prev: \ref main_page "Home" \n
Next: \ref Download "Download"

\page cfg Configuration
\image html rkh_bunner.jpg

Prev: \ref main_page "Home" \n
Next: \ref Usage "Getting started with RKH"

\tableofcontents

Each RKH application must have its own configuration file, called 
\c rkhcfg.h. This file adapts and configures RKH by means of compiler
definitions and macros allowing to restrict the resources consumed by RKH.
Adjusting this definitions allows to reduce the ROM and RAM consumption,
and to enhance the system performance in a substantial manner. 

Also, each RKH port must have its own configuration options, that must be 
explicitely defined in the proper \a rkhport.h file. Here is an list of all 
options with their documentation:

\n This section includes:

- \ref cfg_port
- \ref cfg_fw
- \ref cfg_sm
- \ref cfg_tr
- \ref cfg_q
- \ref cfg_mp
- \ref cfg_t

<HR>
\section cfg_port Configuration options related to RKH port

<TABLE	align="center" valign="middle" width=80% cellspacing=2 
		cellpadding=4 border=0>
	<TR bgColor="#c0c0c0">
		<TH colspan=5 align="left"><B> Related with RKH port </B></TH>
	</TR>
	<TR bgColor="#c0c0c0">
		<TH><B><I> Option </I></B></TH>
		<TH><B><I> Type </I></B></TH>
		<TH><B><I> Range </I></B></TH> 
		<TH><B><I> Default </I></B></TH> 
		<TH><B><I> Description </I></B></TH> 
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFGPORT_SMA_THREAD_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFGPORT_SMA_THREAD_EN </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFGPORT_SMA_THREAD_DATA_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFGPORT_SMA_THREAD_DATA_EN </TD>
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFGPORT_NATIVE_SCHEDULER_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_ENABLED </TD>
		<TD align="left"> \copybrief RKH_CFGPORT_NATIVE_SCHEDULER_EN </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFGPORT_NATIVE_EQUEUE_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_ENABLED </TD>
		<TD align="left"> \copybrief RKH_CFGPORT_NATIVE_EQUEUE_EN </TD>
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFGPORT_NATIVE_DYN_EVT_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_ENABLED </TD>
		<TD align="left"> \copybrief RKH_CFGPORT_NATIVE_DYN_EVT_EN </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFGPORT_REENTRANT_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFGPORT_REENTRANT_EN </TD>
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFGPORT_TRC_SIZEOF_PTR </TD>
		<TD> integer </TD>
		<TD> [8,16,32] </TD>
		<TD> 32 </TD>
		<TD align="left"> \copybrief RKH_CFGPORT_TRC_SIZEOF_PTR </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFGPORT_TRC_SIZEOF_FUN_PTR </TD>
		<TD> integer </TD>
		<TD> [8,16,32] </TD>
		<TD> 32 </TD>
		<TD align="left"> \copybrief RKH_CFGPORT_TRC_SIZEOF_FUN_PTR </TD>
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFGPORT_TRC_SIZEOF_TSTAMP </TD>
		<TD> integer </TD>
		<TD> [8,16,32] </TD>
		<TD> 16 </TD>
		<TD align="left"> \copybrief RKH_CFGPORT_TRC_SIZEOF_TSTAMP </TD>
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFGPORT_SMA_QSTO_EN </TD>
		<TD> boolean </TD>
		<TD> </TD>
		<TD> RKH_ENABLED </TD>
		<TD align="left"> \copybrief RKH_CFGPORT_SMA_QSTO_EN </TD>
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFGPORT_SMA_STK_EN </TD>
		<TD> boolean </TD>
		<TD> </TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFGPORT_SMA_STK_EN </TD>
	</TR>
</TABLE>

Back: \ref cfg "Configuring framework RKH"

<HR>
\section cfg_fw Configuration options related to framework

<TABLE	align="center" valign="middle" width=80% cellspacing=2 
		cellpadding=4 border=0>
	<TR bgColor="#c0c0c0">
		<TH colspan=5 align="left"><B> Related with Framework (FWK)</B></TH>
	</TR>
	<TR bgColor="#c0c0c0">
		<TH><B><I> Option </I></B></TH>
		<TH><B><I> Type </I></B></TH>
		<TH><B><I> Range </I></B></TH> 
		<TH><B><I> Default </I></B></TH> 
		<TH><B><I> Description </I></B></TH> 
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_FWK_MAX_SMA </TD>
		<TD> integer </TD>
		<TD> [1..64] </TD>
		<TD> 4 </TD>
		<TD align="left"> \copybrief RKH_CFG_FWK_MAX_SMA </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_FWK_DYN_EVT_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_FWK_DYN_EVT_EN </TD>
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_FWK_MAX_EVT_POOL </TD>
		<TD> integer </TD>
		<TD> [0..255] </TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_FWK_MAX_EVT_POOL </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_FWK_SIZEOF_EVT </TD>
		<TD> integer </TD>
		<TD> [8,16,32] </TD>
		<TD> 8 </TD>
		<TD align="left"> \copybrief RKH_CFG_FWK_SIZEOF_EVT </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_FWK_MAX_SIGNALS </TD>
		<TD> integer </TD>
		<TD> [1-65536] </TD>
		<TD> 16 </TD>
		<TD align="left"> \copybrief RKH_CFG_FWK_MAX_SIGNALS </TD>
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_FWK_SIZEOF_EVT_SIZE </TD>
		<TD> integer </TD>
		<TD> [8,16,32] </TD>
		<TD> 8 </TD>
		<TD align="left"> \copybrief RKH_CFG_FWK_SIZEOF_EVT_SIZE </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_FWK_DEFER_EVT_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_FWK_DEFER_EVT_EN </TD>
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_FWK_ASSERT_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_ENABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_FWK_ASSERT_EN </TD>
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_FWK_PUBSUB_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_ENABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_FWK_PUBSUB_EN </TD>
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_FWK_MAX_SUBS_CHANNELS </TD>
		<TD> integer </TD>
		<TD> [1..128] </TD>
		<TD> 16 </TD>
		<TD align="left"> \copybrief RKH_CFG_FWK_MAX_SUBS_CHANNELS </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_HOOK_DISPATCH_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_HOOK_DISPATCH_EN </TD>
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_HOOK_SIGNAL_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_HOOK_SIGNAL_EN </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_HOOK_TIMEOUT_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_HOOK_TIMEOUT_EN </TD>
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_HOOK_START_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_ENABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_HOOK_START_EN </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_HOOK_EXIT_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_ENABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_HOOK_EXIT_EN </TD>
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_HOOK_TIMETICK_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_ENABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_HOOK_TIMETICK_EN </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_HOOK_PUT_TRCEVT_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_HOOK_PUT_TRCEVT_EN </TD>
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_FWK_TICK_RATE_HZ </TD>
		<TD> integer </TD>
		<TD> [1..1000] </TD>
		<TD> 100 </TD>
		<TD align="left"> \copybrief RKH_CFG_FWK_TICK_RATE_HZ </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_FWK_AWARE_ISR_PRIO </TD>
		<TD> integer </TD>
		<TD> [0..255] </TD>
		<TD> 0 </TD>
		<TD align="left"> \copybrief RKH_CFG_FWK_AWARE_ISR_PRIO </TD>
	</TR>
</TABLE>

Back: \ref cfg "Configuring framework RKH"

<HR>
\section cfg_sm Configuration options related to state machine applications 

<TABLE	align="center" valign="middle" width=80% cellspacing=2 
		cellpadding=4 border=0>
	<TR bgColor="#c0c0c0">
		<TH colspan=5 align="left"><B> Related with State Machine Application
				SMA or Active Object</B></TH>
	</TR>
	<TR bgColor="#c0c0c0">
		<TH><B><I> Option </I></B></TH>
		<TH><B><I> Type </I></B></TH>
		<TH><B><I> Range </I></B></TH> 
		<TH><B><I> Default </I></B></TH> 
		<TH><B><I> Description </I></B></TH> 
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_SMA_GET_INFO_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_SMA_GET_INFO_EN </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_SMA_PPRO_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_SMA_PPRO_EN </TD>
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_SMA_HCAL_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_ENABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_SMA_HCAL_EN </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_SMA_MAX_HCAL_DEPTH </TD>
		<TD> integer </TD>
		<TD> [1..8] </TD>
		<TD> 4 </TD>
		<TD align="left"> \copybrief RKH_CFG_SMA_MAX_HCAL_DEPTH </TD>
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_SMA_MAX_TRC_SEGS </TD>
		<TD> integer </TD>
		<TD> [1..4] </TD>
		<TD> 4 </TD>
		<TD align="left"> \copybrief RKH_CFG_SMA_MAX_TRC_SEGS </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_SMA_PSEUDOSTATE_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_SMA_PSEUDOSTATE_EN </TD>
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_SMA_DEEP_HIST_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_SMA_DEEP_HIST_EN </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_SMA_SHALLOW_HIST_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_SMA_SHALLOW_HIST_EN </TD>
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_SMA_CHOICE_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_SMA_CHOICE_EN </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_SMA_CONDITIONAL_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_SMA_CONDITIONAL_EN </TD>
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_SMA_SUBMACHINE_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_SMA_SUBMACHINE_EN </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_SMA_TRC_SNDR_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_SMA_TRC_SNDR_EN </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_SMA_INIT_EVT_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_SMA_INIT_EVT_EN </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_SMA_ENT_ARG_SMA_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_ENABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_SMA_ENT_ARG_SMA_EN </TD>
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_SMA_ENT_ARG_STATE_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_SMA_ENT_ARG_STATE_EN </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_SMA_EXT_ARG_SMA_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_ENABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_SMA_EXT_ARG_SMA_EN </TD>
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_SMA_EXT_ARG_STATE_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_SMA_EXT_ARG_STATE_EN </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_SMA_ACT_ARG_SMA_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_ENABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_SMA_ACT_ARG_SMA_EN </TD>
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_SMA_ACT_ARG_EVT_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_ENABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_SMA_ACT_ARG_EVT_EN </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_SMA_GRD_ARG_EVT_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_ENABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_SMA_GRD_ARG_EVT_EN </TD>
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_SMA_GRD_ARG_SMA_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_ENABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_SMA_GRD_ARG_SMA_EN </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_SMA_PPRO_ARG_SMA_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_ENABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_SMA_PPRO_ARG_SMA_EN </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_SMA_SM_CONST_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_ENABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_SMA_SM_CONST_EN </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_SMA_RT_CTOR_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_SMA_RT_CTOR_EN </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_SMA_VFUNCT_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_SMA_VFUNCT_EN </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_SMA_ORTHREG_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_SMA_ORTHREG_EN </TD>
	</TR>
</TABLE>

Back: \ref cfg "Configuring framework RKH"

<HR>
\section cfg_tr Configuration options related to trace facility

<TABLE	align="center" valign="middle" width=80% cellspacing=2 
		cellpadding=4 border=0>
	<TR bgColor="#c0c0c0">
		<TH colspan=5 align="left"><B> Related with trace facility (TRC)</B></TH>
	</TR>
	<TR bgColor="#c0c0c0">
		<TH><B><I> Option </I></B></TH>
		<TH><B><I> Type </I></B></TH>
		<TH><B><I> Range </I></B></TH> 
		<TH><B><I> Default </I></B></TH> 
		<TH><B><I> Description </I></B></TH> 
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_TRC_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_ENABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_TRC_EN </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_TRC_RTFIL_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_TRC_RTFIL_EN </TD>
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_TRC_RTFIL_SMA_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_TRC_RTFIL_SMA_EN </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_TRC_RTFIL_SIGNAL_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_TRC_RTFIL_SIGNAL_EN </TD>
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_TRC_USER_TRACE_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_TRC_USER_TRACE_EN </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_TRC_ALL_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_TRC_ALL_EN </TD>
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_TRC_MP_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_TRC_MP_EN </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_TRC_QUE_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_TRC_QUE_EN </TD>
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_TRC_SMA_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_TRC_SMA_EN </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_TRC_TMR_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_ENABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_TRC_TMR_EN </TD>
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_TRC_SM_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_ENABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_TRC_SM_EN </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_TRC_FWK_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_ENABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_TRC_FWK_EN </TD>
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_TRC_ASSERT_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_ENABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_TRC_ASSERT_EN </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_TRC_SM_INIT_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_TRC_SM_INIT_EN </TD>
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_TRC_SM_DCH_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_TRC_SM_DCH_EN </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_TRC_SM_CLRH_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_TRC_SM_CLRH_EN </TD>
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_TRC_SM_TRN_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_TRC_SM_TRN_EN </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_TRC_SM_STATE_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_ENABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_TRC_SM_STATE_EN </TD>
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_TRC_SM_ENSTATE_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_TRC_SM_ENSTATE_EN </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_TRC_SM_EXSTATE_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_TRC_SM_EXSTATE_EN </TD>
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_TRC_SM_NENEX_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_TRC_SM_NENEX_EN </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_TRC_SM_NTRNACT_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_TRC_SM_NTRNACT_EN </TD>
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_TRC_SM_TS_STATE_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_TRC_SM_TS_STATE_EN </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_TRC_SM_PROCESS_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_TRC_SM_PROCESS_EN </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_TRC_SM_EXE_ACT_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_TRC_SM_EXE_ACT_EN </TD>
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_TRC_NSEQ_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_TRC_NSEQ_EN </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_TRC_CHK_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_ENABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_TRC_CHK_EN </TD>
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_TRC_TSTAMP_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_ENABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_TRC_TSTAMP_EN </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_TRC_SIZEOF_STREAM </TD>
		<TD> integer </TD>
		<TD> [8,16,32] </TD>
		<TD> 128 </TD>
		<TD align="left"> \copybrief RKH_CFG_TRC_SIZEOF_STREAM </TD>
	</TR>
</TABLE>

Back: \ref cfg "Configuring framework RKH"

<HR>
\section cfg_q Configuration options related to queue facility

<TABLE	align="center" valign="middle" width=80% cellspacing=2 
		cellpadding=4 border=0>
	<TR bgColor="#c0c0c0">
		<TH colspan=5 align="left"><B> Related with queue module (QUE)</B></TH>
	</TR>
	<TR bgColor="#c0c0c0">
		<TH><B><I> Option </I></B></TH>
		<TH><B><I> Type </I></B></TH>
		<TH><B><I> Range </I></B></TH> 
		<TH><B><I> Default </I></B></TH> 
		<TH><B><I> Description </I></B></TH> 
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_QUE_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_ENABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_QUE_EN </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_QUE_SIZEOF_NELEM </TD>
		<TD> integer </TD>
		<TD> [8,16,32] </TD>
		<TD> 8 </TD>
		<TD align="left"> \copybrief RKH_CFG_QUE_SIZEOF_NELEM </TD>
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_QUE_GET_LWMARK_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_ENABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_QUE_GET_LWMARK_EN </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_QUE_READ_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_QUE_READ_EN </TD>
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_QUE_DEPLETE_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_QUE_DEPLETE_EN </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_QUE_IS_FULL_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_QUE_IS_FULL_EN </TD>
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_QUE_GET_NELEMS_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_QUE_GET_NELEMS_EN </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_QUE_PUT_LIFO_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_QUE_PUT_LIFO_EN </TD>
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_QUE_GET_INFO_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_QUE_GET_INFO_EN </TD>
	</TR>
</TABLE>

Back: \ref cfg "Configuring framework RKH"

<HR>
\section cfg_mp Configuration options related to fixed-sized memory block facility

<TABLE	align="center" valign="middle" width=80% cellspacing=2 
		cellpadding=4 border=0>
	<TR bgColor="#c0c0c0">
		<TH colspan=5 align="left"><B> Related with fixed-sized memory block 
										facility (MP)</B></TH>
	</TR>
	<TR bgColor="#c0c0c0">
		<TH><B><I> Option </I></B></TH>
		<TH><B><I> Type </I></B></TH>
		<TH><B><I> Range </I></B></TH> 
		<TH><B><I> Default </I></B></TH> 
		<TH><B><I> Description </I></B></TH> 
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_MP_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_MP_EN </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_MP_REDUCED_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_MP_REDUCED_EN </TD>
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_MP_SIZEOF_BSIZE </TD>
		<TD> integer </TD>
		<TD> [8,16,32] </TD>
		<TD> 8 </TD>
		<TD align="left"> \copybrief RKH_CFG_MP_SIZEOF_BSIZE </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_MP_SIZEOF_NBLOCK </TD>
		<TD> integer </TD>
		<TD> [8,16,32] </TD>
		<TD> 8 </TD>
		<TD align="left"> \copybrief RKH_CFG_MP_SIZEOF_NBLOCK </TD>
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_MP_GET_BSIZE_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_MP_GET_BSIZE_EN </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_MP_GET_NFREE_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_MP_GET_NFREE_EN </TD>
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_MP_GET_LWM_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_MP_GET_LWM_EN </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_MP_GET_INFO_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_MP_GET_INFO_EN </TD>
	</TR>
</TABLE>

Back: \ref cfg "Configuring framework RKH"

<HR>
\section cfg_t Configuration options related to software timer facility

<TABLE	align="center" valign="middle" width=80% cellspacing=2 
		cellpadding=4 border=0>
	<TR bgColor="#c0c0c0">
		<TH colspan=5 align="left"><B> Related with software timer facility 
																(TMR)</B></TH>
	</TR>
	<TR bgColor="#c0c0c0">
		<TH><B><I> Option </I></B></TH>
		<TH><B><I> Type </I></B></TH>
		<TH><B><I> Range </I></B></TH> 
		<TH><B><I> Default </I></B></TH> 
		<TH><B><I> Description </I></B></TH> 
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_TMR_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_ENABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_TMR_EN </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_TMR_SIZEOF_NTIMER </TD>
		<TD> integer </TD>
		<TD> [8,16,32] </TD>
		<TD> 8 </TD>
		<TD align="left"> \copybrief RKH_CFG_TMR_SIZEOF_NTIMER </TD>
	</TR>
	<TR bgColor="#f0f0f0" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_TMR_HOOK_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_TMR_HOOK_EN </TD>
	</TR>
	<TR bgColor="#c8cedc" align="center" valign="middle" >
		<TD align="left"> #RKH_CFG_TMR_GET_INFO_EN </TD>
		<TD> boolean </TD>
		<TD></TD>
		<TD> RKH_DISABLED </TD>
		<TD align="left"> \copybrief RKH_CFG_TMR_GET_INFO_EN </TD>
	</TR>
</TABLE>

Prev: \ref main_page "Home" \n
Next: \ref Usage "Getting started with RKH"
*/


/**
\page Usage Getting started with RKH
\image html rkh_bunner.jpg

Prev: \ref main_page "Home" \n
Next: \ref cmake.md "Application development using CMake"

The goal in this section is to explain how to represent a state machine 
using the RKH framework. To do that is proposed a simple example, which is 
shown in the \ref fig1 "Figure 1". Also, this section summarizes the main 
rules and concepts for making the most out of RKH features.

\image html rkh_framework_3.png "RKH Software Architecture"

\note
Although the diagram shown in the \ref fig1 "Figure 1" does not have a 
submachine state, was considered appropriate to include in this section a 
detailed explanation of how to deal with this very powerful concept using 
the RKH framework.
The purpose of defining submachine states is to decompose and localize 
repetitive parts because the same state machine can be referenced from 
more than one submachine state.

\n This section includes:

- \subpage basics
- \subpage preparing
- \subpage identify_events
- \subpage identify
- \subpage representing
- \subpage running
- \subpage submachine

\tableofcontents

\anchor fig1
\image html my.png "Figure 1 - \"my\" state diagram"

*/
