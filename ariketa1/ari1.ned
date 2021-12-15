//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2003-2015 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//
import ned.DatarateChannel;

simple Tic
{
    parameters:
        @display("i=block/routing");
    gates:
        input in;
        output out;
}

simple Toc
{
    parameters:
        @display("i=block/process");
    gates:
        input in;
        output out;
}

network Tictoc
{
    @display("bgb=337,229");
    types:
        channel C extends DatarateChannel
        {
            datarate = 1000bps;
            per = 0.05;
            delay = 0.1ms;
        }
    submodules:
        tic: Tic {
            parameters:
                @display("i=,cyan;p=57,62");
        }
        toc: Toc {
            parameters:
                @display("i=,gold;p=247,154");
        }
    connections:
        tic.out --> C --> toc.in;
        tic.in <-- C <-- toc.out;
}
