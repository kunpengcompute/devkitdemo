package io.jenkins.plugins.devkit.slavecopy;

import hudson.Util;
import hudson.util.DirScanner;
import hudson.util.FileVisitor;

import org.apache.tools.ant.DirectoryScanner;
import org.apache.tools.ant.Project;
import org.apache.tools.ant.types.FileSet;

import java.io.File;
import java.io.IOException;


/**
 * Modified copy of  Hudson's {@link DirScanner.Glob} to modify the {@code scan()}
 * method so that it takes into account Ant's default excludes (cf. HUDSON-7999).
 */
public class MyGlobDirScanner extends DirScanner {

    private final String includes;

    MyGlobDirScanner(final String includes) {
        this.includes = includes;
    }

    public void scan(File dir, FileVisitor visitor) throws IOException {
        if(Util.fixEmpty(includes)==null) {
            // optimization
            new Full().scan(dir, visitor);
            return;
        }

        FileSet fs = Util.createFileSet(dir,includes);

        if(dir.exists()) {
            DirectoryScanner ds = fs.getDirectoryScanner(new Project());
            for(String f : ds.getIncludedFiles()) {
                File file = new File(dir, f);
                visitor.visit(file,f);
            }
        }
    }

}