package com.ipoleksenko.sense;

import android.content.ContentProvider;
import android.content.ContentValues;
import android.content.UriMatcher;
import android.database.Cursor;
import android.database.MatrixCursor;
import android.net.Uri;
import android.os.Environment;
import android.util.Log;
import android.webkit.MimeTypeMap;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import java.io.File;
import java.io.IOException;

public class MediaFileProvider extends ContentProvider {

    public static final String AUTHORITY = "com.ipoleksenko.sense.provider";
    private static final UriMatcher uriMatcher = new UriMatcher(UriMatcher.NO_MATCH);
    private static final int FILE = 1;

    static {
        uriMatcher.addURI(AUTHORITY, "**", FILE);
    }

    @Override
    public boolean onCreate() {
        return true;
    }

    @Nullable
    @Override
    public String getType(@NonNull Uri uri) {
        String path = uri.getLastPathSegment();
        String ext = MimeTypeMap.getFileExtensionFromUrl(path);
        return MimeTypeMap.getSingleton().getMimeTypeFromExtension(ext);
    }

    @Nullable
    @Override
    public Cursor query(@NonNull Uri uri, @Nullable String[] projection,
                        @Nullable String selection, @Nullable String[] selectionArgs,
                        @Nullable String sortOrder) {

        MatrixCursor cursor = new MatrixCursor(new String[]{"name", "path", "size"});
        File dir = new File(Environment.getExternalStorageDirectory(), "Android/media/com.ipoleksenko.sense");

        if (dir.exists()) {
            for (File file : dir.listFiles()) {
                cursor.addRow(new Object[]{file.getName(), file.getAbsolutePath(), file.length()});
            }
        }

        return cursor;
    }

    @Nullable
    @Override
    public Uri insert(@NonNull Uri uri, @Nullable ContentValues values) {
        String fileName = values != null ? values.getAsString("name") : uri.getLastPathSegment();
        if (fileName == null) return null;

        File mediaDir = new File(Environment.getExternalStorageDirectory(),
                "Android/media/com.ipoleksenko.sense");
        if (!mediaDir.exists()) mediaDir.mkdirs();

        File newFile = new File(mediaDir, fileName);
        try {
            if (newFile.createNewFile()) {
                return Uri.parse("content://" + AUTHORITY + "/" + fileName);
            } else {
                return null;
            }
        } catch (IOException e) {
            Log.e("MediaFileProvider", "Failed to create new file", e);
            return null;
        }
    }

    @Override
    public int delete(@NonNull Uri uri, @Nullable String selection,
                      @Nullable String[] selectionArgs) {
        File file = new File(getFilePath(uri));
        return file.delete() ? 1 : 0;
    }

    @Override
    public int update(@NonNull Uri uri, @Nullable ContentValues values,
                    @Nullable String selection, @Nullable String[] selectionArgs) {
        String newName = values != null ? values.getAsString("new_name") : null;
        if (newName == null) return 0;

        File oldFile = new File(getFilePath(uri));
        File newFile = new File(oldFile.getParent(), newName);

        if (oldFile.exists() && oldFile.renameTo(newFile)) {
            return 1;
        } else {
            return 0;
        }
    }

    @Nullable
    @Override
    public android.os.ParcelFileDescriptor openFile(@NonNull Uri uri, @NonNull String mode)
            throws java.io.FileNotFoundException {

        File file = new File(getFilePath(uri));
        return android.os.ParcelFileDescriptor.open(file,
                "r".equals(mode)
                        ? android.os.ParcelFileDescriptor.MODE_READ_ONLY
                        : android.os.ParcelFileDescriptor.MODE_READ_WRITE);
    }

    @Nullable
    private String getFilePath(Uri uri) {
        String relativePath = uri.getPath();
        if (relativePath == null) return null;

        relativePath = relativePath.replaceFirst("^/", "");

        File mediaDir = new File(Environment.getExternalStorageDirectory(),
                "Android/media/com.ipoleksenko.sense");

        File fullPath = new File(mediaDir, relativePath);
        File parent = fullPath.getParentFile();
        if (parent != null && !parent.exists()) parent.mkdirs();

        return fullPath.getAbsolutePath();
    }
}
