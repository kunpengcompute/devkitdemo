package io.jenkins.plugins.devkit.slavecopy;

import hudson.FilePath;
import hudson.Util;
import hudson.remoting.Future;
import hudson.remoting.Pipe;
import hudson.remoting.VirtualChannel;
import hudson.util.IOUtils;
import hudson.util.io.Archiver;
import hudson.util.io.ArchiverFactory;

import org.apache.commons.lang.StringUtils;
import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.Project;
import org.apache.tools.ant.taskdefs.Copy;
import org.apache.tools.ant.types.FileSet;
import org.apache.tools.tar.TarEntry;
import org.apache.tools.tar.TarInputStream;
import org.jenkinsci.remoting.RoleChecker;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.Serializable;
import java.util.concurrent.ExecutionException;

/**
 * The MyFilePath
 *
 *
 *
 *
 */
public class MyFilePath implements Serializable {
    private static final long serialVersionUID = 1L;

    public static int copyTo(final FilePath source, final String includes, final boolean flatten, final FilePath target)
        throws IOException, InterruptedException {
        if (source.getChannel() == target.getChannel()) {
            // --- local -->local copy ---
            return new FilePath.FileCallable<Integer>() {
                @Override
                public void checkRoles(RoleChecker checker) throws SecurityException {
                }

                @Override
                public Integer invoke(File sourceBaseDir, VirtualChannel channel)
                    throws IOException, InterruptedException {
                    if (!sourceBaseDir.exists()) {
                        return 0;
                    }
                    try {
                        class CopyImpl extends Copy {
                            private int copySize;

                            CopyImpl() {
                                setProject(new Project());
                            }

                            @Override
                            protected void doFileOperations() {
                                copySize = super.fileCopyMap.size();
                                super.doFileOperations();
                            }

                            public int getNumCopied() {
                                return copySize;
                            }
                        }
                        FileSet fs = Util.createFileSet(sourceBaseDir, includes);

                        CopyImpl copyTask = new CopyImpl();
                        copyTask.setTodir(new File(target.getRemote()));
                        copyTask.addFileset(fs);
                        copyTask.setOverwrite(false);
                        copyTask.setIncludeEmptyDirs(false);
                        copyTask.setFlatten(flatten);

                        copyTask.execute();
                        return copyTask.getNumCopied();
                    } catch (BuildException e) {
                        throw new IOException("Failed to copy from " + sourceBaseDir + " to " + target, e);
                    }
                }
            } .invoke(new File(source.getRemote()), FilePath.localChannel);
        }else {
            // local --> remote copy
            final Pipe pipe = Pipe.createLocalToRemote();

            Future<Void> future = target.actAsync(new FilePath.FileCallable<Void>() {
                private static final long serialVersionUID = 1L;

                @Override
                public Void invoke(File f, VirtualChannel channel) throws IOException, InterruptedException {
                    try {
                        readFromTar(f, flatten, FilePath.TarCompression.GZIP.extract(pipe.getIn()));
                        return null;
                    } finally {
                        pipe.getIn().close();
                    }
                }

                @Override
                public void checkRoles(RoleChecker checker) throws SecurityException {
                }

            });
            int r = writeToTar(new File(source.getRemote()), includes,
                FilePath.TarCompression.GZIP.compress(pipe.getOut()));
            try {
                future.get();
            } catch (ExecutionException e) {
                throw new IOException(e);
            }
            return r;
        }
    }

    private static void readFromTar(File baseDir, boolean flatten, InputStream in) throws IOException {
        TarInputStream t = new TarInputStream(in);
        try {
            TarEntry tarEntry;
            while ((tarEntry = t.getNextEntry()) != null) {
                File f = null;
                if (!flatten || (!tarEntry.getName().contains("/") && !tarEntry.getName().contains("\\"))) {
                    f = new File(baseDir, tarEntry.getName());
                } else {
                    String fileName = StringUtils.substringAfterLast(tarEntry.getName(), "/");
                    if (StringUtils.isBlank(fileName)) {
                        fileName = StringUtils.substringAfterLast(tarEntry.getName(), "\\");
                    }
                    f = new File(baseDir, fileName);
                }
                if (!flatten && tarEntry.isDirectory()) {
                    f.mkdirs();
                } else {
                    if (!flatten && f.getParentFile() != null) {
                        f.getParentFile().mkdirs();
                    }

                    IOUtils.copy(t, f);
                    //未设置读写权限
                }
            }
        } catch (IOException e) {
            throw new IOException("Failed to extract to " + baseDir.getAbsolutePath(), e);
        } finally {
            t.close();
        }
    }

    private static Integer writeToTar(File baseDir, String includes, OutputStream out) throws IOException {
        Archiver tw = ArchiverFactory.TAR.create(out);
        try {
            new MyGlobDirScanner(includes).scan(baseDir, tw);
        } finally {
            tw.close();
        }
        return tw.countEntries();
    }
}
