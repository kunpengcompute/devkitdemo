/*
 * Revise from https://github.com/openjdk/jdk17u-dev/blob/master/test/hotspot/jtreg/vmTestbase/jit/deoptimization/test01/test01.java
 *
 * Copyright (c) 2008, 2020, Oracle and/or its affiliates. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 */

import java.lang.management.ManagementFactory;

public class Main {
    public static void main(String[] args) throws ClassNotFoundException {
        String name = ManagementFactory.getRuntimeMXBean().getName();
        String processId = name.split("@")[0];
        System.out.println("current process ID: " + processId);
        try { 
            Thread.sleep(15000);
            A obj = new A();
            for (int i = 0; i < 100000; i++) {
                obj.used_alot();
            } 
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}

class A {
    protected final int counter = 250000;
    protected int count = 0;
    public void foo(int index) {
        count++;
        if (index == counter - 1) {
            try {
                ((B)Class.forName("B").newInstance()).foo(index);
            } catch(Exception e) {
            }
        }
    }

    public void used_alot() {
        for (int index = 0; index < counter; index++) {
            foo(index);
        }
    }
}

class B extends A {
  public void foo(int index) {
    count--;
  }
};
