package io.jenkins.plugins.devkit.utils;

import hudson.FilePath;
import hudson.model.TaskListener;

import java.io.File;
import java.io.IOException;
import java.nio.charset.StandardCharsets;

public class HtmlRepoFiles {

    private HtmlRepoFiles() {
    }

    private static class CreateRepoFilesHolder {
        private static HtmlRepoFiles INSTANCE = new HtmlRepoFiles();
    }

    public static HtmlRepoFiles getInstance() {
        return CreateRepoFilesHolder.INSTANCE;
    }

    public void writeTo(FilePath repoWorkspace, String repoTaskID, String repoContext, TaskListener listener)
            throws IOException, InterruptedException {
        FilePath filePath = repoWorkspace.withSuffix(File.separator + repoTaskID + "_report.html");
        filePath.write(repoContext, StandardCharsets.UTF_8.toString());
        listener.getLogger().println("本地扫描报告路径： " + filePath);
    }

}
