package io.jenkins.plugins.devkit.utils;

import hudson.model.TaskListener;

import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;

public class BashCmdExec {
    public static int callProcessBuilder(String userCommand, TaskListener listener) {
        List<String> command = new ArrayList<String>();
        command.add("sh");
        command.add("-c");
        command.add(userCommand);
        listener.getLogger().println("command is: " + command);
        ProcessBuilder processBuilder = new ProcessBuilder();
        processBuilder.command(command);
        // 将标准输入流和错误输入流合并，通过标准输入流读取信息
        processBuilder.redirectErrorStream(true);
        int exitValue = -1;
        try {
            //启动进程
            Process start = processBuilder.start();
            //获取输入流
            InputStream inputStream = start.getInputStream();
            //转成字符输入流
            InputStreamReader inputStreamReader = new InputStreamReader(inputStream, "UTF-8");
            int len = -1;
            char[] c = new char[1024];
            StringBuffer outputString = new StringBuffer();
            //读取进程输入流中的内容
            while ((len = inputStreamReader.read(c)) != -1) {
                String s = new String(c, 0, len);
                outputString.append(s);
                listener.getLogger().println(s);
            }
            try {
                exitValue = start.waitFor();
                if (exitValue != 0 ) {
                    listener.error("Exec" + command + "failed!");
                }
            } catch (InterruptedException e) {
                throw new RuntimeException(e);
            }
            inputStreamReader.close();
            inputStream.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return exitValue;
    }

}
