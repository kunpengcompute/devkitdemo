/*
 * Copyright 2022 Huawei Technologies Co., Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * Description: haf demo
 * Create: 2022-07-24
 */
package com.demo;

import com.huawei.haf.annotations.HafTarget;

public class HafDemo {
    private int c = 0;
    private String ip = "127.0.0.1";
    // target 卸载的目标服务器地址 必选
    // to 指定传入得参数 可选
    // from 卸载测需要返回的变量 可选
    // timeout 超时时间，默认为1800秒 可选
    // runInHostIfFailed 如果执行失败是否在主机测执行，默认为true 可选
    // exception 指定异常处理函数，默认为执行失败后本地执行  可选
    // app 卸载的应用名称，默认为jar包名称 可选
    @HafTarget(target = ip, to = {"a", "b"}, from = {"c"}, timeout = 600, runInHostIfFailed = true, exception = Exception.exception, app = "add")
    int sum(int a, int b) {
        c = 100;
        return a + b;
    }
    public static void main(String[] args) {
        int numA = 0;
        int numB = 0;
        try {
            numA = Integer.valueOf(args[0]).intValue();
            numB = Integer.valueOf(args[1]).intValue();
        } catch (Exception e) {
            System.out.println(e.fillInStackTrace());
            return;
        }
        HafDemo haf = new HafDemo();
        int sum = haf.sum(numA, numB);
        System.out.println(numA + " + " + numB + " = " + sum);
        System.out.println("The c value is " + haf.c);
    }
}

