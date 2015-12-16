
package fr.jbflamant.androidusbaccess;

import android.content.Context;
import java.io.File;
import java.io.BufferedOutputStream;
import java.io.BufferedInputStream;
import java.io.FileInputStream;
import java.io.IOException;
import android.net.Uri;


public class HardwareUtils extends org.qtproject.qt5.android.bindings.QtActivity
{
    private static HardwareUtils _context;

    public HardwareUtils()
    {
        _context = this;
    }

    private void copyFile(File src, Uri destUri) {
        BufferedInputStream bis = null;
        BufferedOutputStream bos = null;

        try {
            bis = new BufferedInputStream(new FileInputStream(src));
            bos = new BufferedOutputStream(getContentResolver().openOutputStream(destUri));
            byte[] buf = new byte[1024];
            bis.read(buf);
            do {
                bos.write(buf);
            } while(bis.read(buf) != -1);
        } catch (NullPointerException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                if (bis != null) bis.close();
                if (bos != null) bos.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}
