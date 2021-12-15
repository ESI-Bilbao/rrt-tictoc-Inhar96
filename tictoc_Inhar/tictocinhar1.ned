//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2003-2015 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//
import ned.DatarateChannel;

simple Tic8
{
    parameters:
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
simple source
{
    parameters:
        @display("i=block/source");
    gates:
        output out;
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
        sar1: Tic8 {
            parameters:
                @display("i=,cyan;p=144,156");
        }
        sar2: Tic8 {
            parameters:
                @display("i=,cyan;p=289,62");
        }
        sar5: Tic8 {
            parameters:
                @display("i=,cyan;p=289,230");
        }
        nod3: Toc8 {
            parameters:
                @display("i=,gold;p=406,62");
        }
        nod4: Toc8 {
            parameters:
                @display("i=,gold;p=415,230");
        }
        itu1: source {
            @display("p=66,156");
        }
        itu2: source {
            @display("p=105,62");
        }
        itu3: source {
            @display("p=171,249");
        }
    connections:

        itu1.out --> sar1.in++;
        itu2.out --> sar2.in++;
        itu3.out --> sar5.in++;

        sar1.out++ --> C --> sar2.in++;
        sar2.out++ --> C --> sar1.in++;

        sar1.out++ --> C --> sar5.in++;
        sar5.out++ --> C --> sar1.in++;

        sar2.out++ --> C --> nod3.in++;
        nod3.out++ --> C --> sar2.in++;

        sar2.out++ --> C --> nod4.in++;
        nod4.out++ --> C --> sar2.in++;

        sar5.out++ --> C --> sar2.in++;
        sar2.out++ --> C --> sar5.in++;

        sar5.out++ --> C --> nod4.in++;
        nod4.out++ --> C --> sar5.in++;



}
