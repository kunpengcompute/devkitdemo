package io.jenkins.plugins.devkit.utils;

import hudson.FilePath;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

public class CreateRepoFiles {

    private CreateRepoFiles() {
    }

    private static class CreateRepoFilesHolder {
        private static CreateRepoFiles INSTANCE = new CreateRepoFiles();
    }

    public static CreateRepoFiles getInstance() {
        return CreateRepoFilesHolder.INSTANCE;
    }

    public Path getPath(FilePath repoWorkspace, String repoTaskID) {
        String repoName = repoTaskID + "_report.html";
        Path repoWorkspacePath = Paths.get(repoWorkspace.toString());
        if (Files.exists(repoWorkspacePath)) {
            return repoWorkspacePath.resolve(repoName);
        } else {
            try {
                return Files.createDirectories(repoWorkspacePath).resolve(repoName);
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        }
    }

}
