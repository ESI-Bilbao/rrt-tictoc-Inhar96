//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2003-2015 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//
import ned.DatarateChannel;

simple source
{
    parameters:
        @display("i=block/source");
    gates:
        output out;
}
simple Tic8
{
    parameters:
        double prob1;
        @display("i=block/routing");
    gates:
        input in[];
        output out[];
}

simple Toc8
{
    parameters:
        @display("i=block/process");
    gates:
        input in[];
        output out[];
}

network Tictoc8
{
    @display("bgb=549,319");
    types:
        channel C extends DatarateChannel
        {
            datarate = 1000bps;
            per = 0.5;
            delay = 0.1ms;
        }
    submodules:
        itu1: source {
            @display("p=60,150");
        }
        itu2: source {
            @display("p=105,62");
        }
        itu3: source {
            @display("p=171,249");
        }
        sar1: Tic8 {
            parameters:
                @display("i=,cyan;p=144,156");
            gates:
                in[3];
                out[2]; 
        }
        sar2: Tic8 {
            parameters:
                @display("i=,cyan;p=289,62");
                gates:
                in[5];
                out[4];
        }
        sar5: Tic8 {
            parameters:
                @display("i=,cyan;p=289,230");
                gates:
                in[4];
                out[3];
        }
        nod3: Toc8 {
            parameters:
                @display("i=,gold;p=406,62");
                gates:
                in[2];
                out[2];
        }
        nod4: Toc8 {
            parameters:
                @display("i=,gold;p=415,230");
                gates:
                in[3];
                out[3];
        }

    connections:

        itu1.out --> sar1.in[2];
        itu2.out --> sar2.in[4];
        itu3.out --> sar5.in[3];

        sar1.out[0] --> C --> sar2.in[3];
        sar2.out[3] --> C --> sar1.in[0];

        sar1.out[1] --> C --> sar5.in[2];
        sar5.out[2] --> C --> sar1.in[1];

        sar2.out[0] --> C --> nod3.in[0];
        nod3.out[0] --> C --> sar2.in[0];

        sar2.out[1] --> C --> nod4.in[0];
        nod4.out[0] --> C --> sar2.in[1];

        sar5.out[0] --> C --> sar2.in[2];
        sar2.out[2] --> C --> sar5.in[0];

        sar5.out[1] --> C --> nod4.in[1];
        nod4.out[1] --> C --> sar5.in[1];
        
        nod3.out[1] --> C --> nod4.in[2];
        nod4.out[2] --> C --> nod3.in[1];
        
}
