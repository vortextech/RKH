# *Shared* example

## Description:

This application deals with the shared resource problem in active object systems. Showing one of the biggest benefit of using active objects: resource encapsulation.

The encapsulation naturally designates the owner of the resource as the ultimate arbiter in resolving any contention and potential conflicts for the resource.

The *Shared* application is relatively simple and can be tested only with a couple of LEDs on your target board.

Still, *Shared* contains five (5) concurrent active objects that exchange events via direct event posting mechanism.

The application uses four timers, as well as dynamic and static events.
On the other hand, this application could be used in either preemptive or cooperative environment.

Additionally, the *Shared* could be used to verify a new RKH port.


## *BSP*

As it was said before, this application is the current *"Gold Standard"* to test the proper working of a newly developed port. For this reason, this example (or *demo*) can be compiled for multiple platforms. In this case, it's to validate the FreeRTOS port but many others can be found in the RKH/demo tree.

Even though this port can run in multiple hardware targets depending in FreeRTOS's compatibility, in this particular case it is used the NXP LPC4337 based CIAA board (in either educational or full version) for convenience.

## IDE & Compiling

This project was developed and tested on [MCUXpresso](https://www.nxp.com/design/software/development-software/mcuxpresso-software-and-tools-/mcuxpresso-integrated-development-environment-ide:MCUXpresso-IDE) in order to simplify the process with all the embedded tools it contains. As it's an *Eclipse IDE* after all, you can use it too provided that you make the necessary changes regarding the *toolchain* used to compile the code.

### Import the project

Just go to `File->Import` and select the `Import Projects from File System or Archive` option. Point to the `mcuxpresso` directory located where this README is located and proceed to import the project listed.

### Platform specific parameters

To differ the compiling process to one of the mentioned platforms, an environment variable is used. This variable is `BOARD`, selected for compatibility with some board-specific code.

To select the platform, there is no need to change the variable's value. Just **select the build configuration desired** in `Project->Build Configurations->Set Active`.

### RKH related memory usage

Next we can see a table depicting the memory usage by RKH modules for this demo including the *trace* capability. All values shown are in bytes.

| Module         | .bss | .text | .data | .rodata |
|----------------|-----:|------:|------:|--------:|
| rkhfwk_bittbl  |    0 |    44 |     0 |       8 |
| rkhfwk_dynevt  |   36 |   860 |     0 |      16 |
| rkhfwk_evtpool |   80 |   344 |     0 |      16 |
| rkhfwk_pubsub  |    0 |     0 |     0 |       0 |
| rkhfwk_rdygrp  |    0 |     0 |     0 |       0 |
| rkhfwk_sched   |    0 |     0 |     0 |       0 |
| rkhfwk_version |    0 |     0 |     0 |       8 |
| rkhmempool     |    0 |   760 |     0 |      12 |
| rkhqueue       |   12 |   776 |     0 |      12 |
| rkhsm          |    0 |  3016 |     0 |      16 |
| rkhsma         |   32 |   672 |     0 |       8 |
| rkhsma_prio    |    0 |     0 |     0 |       0 |
| rkhsma_sync    |    0 |     0 |     0 |       0 |
| rkhtmr         |    4 |   976 |     0 |       8 |
| rkhtrc_filter  |   26 |   784 |     0 |      48 |
| rkhtrc_record  |    2 |   812 |     0 |       0 |
| rkhtrc_stream  |  528 |   336 |     0 |       0 |

Next the same analysis without *trace*.

| Module         | .bss | .text | .data | .rodata |
|----------------|-----:|------:|------:|--------:|
| rkhfwk_bittbl  |    0 |     0 |     0 |       0 |
| rkhfwk_dynevt  |   36 |   456 |     0 |      16 |
| rkhfwk_evtpool |   80 |   216 |     0 |      16 |
| rkhfwk_pubsub  |    0 |     0 |     0 |       0 |
| rkhfwk_rdygrp  |    0 |     0 |     0 |       0 |
| rkhfwk_sched   |    0 |     0 |     0 |       0 |
| rkhfwk_version |    0 |     0 |     0 |       8 |
| rkhmempool     |    0 |   576 |     0 |      12 |
| rkhqueue       |   12 |   508 |     0 |      12 |
| rkhsm          |    0 |  1200 |     0 |      16 |
| rkhsma         |   32 |   472 |     0 |       8 |
| rkhsma_prio    |    0 |     0 |     0 |       0 |
| rkhsma_sync    |    0 |     0 |     0 |       0 |
| rkhtmr         |    4 |   684 |     0 |       8 |
| rkhtrc_filter  |    0 |     0 |     0 |       0 |
| rkhtrc_record  |    0 |     0 |     0 |       0 |
| rkhtrc_stream  |    0 |     0 |     0 |       0 |

You can see the small footprint added by the framework.

## Running/Debugging the application


### Interface
To debug the application, it is used the board included programming/debugging interface. It's based on the FTDI FT2232H, the rather well known IC that exposes a JTAG interface and a UART through USB.

The JTAG is managed by OpenOCD and for this, the corresponding script is provided.

### Starting the session in the IDE
Included in the project there are two launchers (and one more as a legacy/backup) that will initiate the tools embedded in MCUXpresso (Check the [documentation](https://www.nxp.com/docs/en/quick-reference-guide/MCUXpresso_IDE_FreeRTOS_Debug_Guide.pdf) about this subject). One for each platform previously said.

The only difference between each launcher is an argument passed to the OpenOCD program to deal with the specific target.

The launchers will start an instance of OpenOCD and GDB. They will also inform to the OpenOCD that it has to be aware of the presence of FreeRTOS so we can get the full benefits of this tool. To see more details of this, please go to [OpenOCD's documentation](http://openocd.org/doc/html/GDB-and-OpenOCD.html#RTOS-Support).

As we configure the OpenOCD in this way, it will inform to the GDB the "presence" of the rtos threads and they will be listed independently during the debug session. But it has a drawback: it will fetch a structure that informs about the threads and if it's not created already it will crash. To prevent this, the temporary breakpoint normally set on main is removed. **If it's not removed the debug session will fail at start**. In the launchers this temporary breakpoint was moved to `vTaskStartScheduler` as it is a safe location to infer that the according structure is already created and populated.

In case that you need to debug the initialization or task creation process, we recommend to tailor the third debug launcher to your needs as it will not configure the OpenOCD to be RTOS-aware. This will not try to list the threads and a breakpoint can be placed on the main to debug this section of the application. Be aware that a temporary breakpoint can persist even if you change the debug configuration.

### Interfacing with the app and the RKH framework

The app will have a "console output" using semihosting through the JTAG interface. It will show the banner or "splash" information of the demo and the messages generated to inform the client-server interaction.

Also, if the trace capability is enabled, the according output is channelized through the UART to the COM/TTY interface. In this case you can run [Trazer](https://github.com/vortexmakes/Trazer) to interpret it.

In case that you are using Linux, you can launch it with:
```sh
./trazer -c /dev/ttyUSB1 19200 8N1
```

This tool will allow you to store the trace in a file using the `-o` argument and you can analyse it later on in a tool like [Trace Compass](https://github.com/vortexmakes/TrazerTC).

### Demo workings

You will notice that this demo is using FreeRTOS to encapsulate each *Active Object* in a independent thread. To communicate between them, the artifacts of FreeRTOS (e.g. queues, etc) are used.

This is **not** the only way you can integrate RKH and FreeRTOS in a project. For example, you can dedicate a single thread to the framework and use RKH's scheduler an IPCs. This will be left to the reader to explore.

### Demo hardware interaction

As this example can be compiled either for CIAA-NXP or EDU-CIAA-NXP platforms, the means to interact with the app running differ:

Each Client has its own LED to show when the server is running its request. The assignment is according to the next table.

|   Platform   | Client 1 | Client 2 | Client 3 | Client 4 |
|:------------:|:--------:|:--------:|:--------:|:--------:|
|   CIAA-NXP   |   DOUT4  |   DOUT5  |   DOUT6  |   DOUT7  |
| EDU-CIAA-NXP |   LEDB   |   LED1   |   LED2   |   LED3   |

To pause the server, one has to drive the `DIN0` input or press the `TEC1` button depending if you are using the CIAA-NXP or EDU-CIAA-NXP respectively. This will generate a pause `event`.
When the server is paused, this situation will be indicated lighting **all** the Client's LEDs at the same time.

To resume from the pause, just press or activate again the previous input.