import java.io.BufferedWriter;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Arrays;

public class Bitmap {

    private final Path file = Paths.get("/tmp/ep3.mem");
    private final Path virtualFile = Paths.get("/tmp/ep3.vir");

    private int [] bitMap;
    private int [] virtualBitMap;

    public Bitmap(int size, int virtualSize) throws IOException {
        initFile(file, size, bitMap);
        initFile(virtualFile, virtualSize, virtualBitMap);
    }

    public void updateBitMap() throws IOException {
        BufferedWriter writer = Files.newBufferedWriter(file);
        writer.write(Arrays.toString(bitMap));

        BufferedWriter virtualWriter = Files.newBufferedWriter(virtualFile);
        virtualWriter.write(Arrays.toString(virtualBitMap));
//
//        String[] stringBitMap = Files.lines(file).findFirst().get().split(" ");
//        for (int i = 0; i < stringBitMap.length; i++) {
//            bitMap[i] = Integer.valueOf(stringBitMap[i]);
//        }
//
//        String[] stringVirtualBitMap = Files.lines(virtualFile).findFirst().get().split(" ");
//        for (int i = 0; i < stringVirtualBitMap.length; i++) {
//            virtualBitMap[i] = Integer.valueOf(stringVirtualBitMap[i]);
//        }
    }

    public int[] getBitMap() {
        return bitMap;
    }

    public void setBitMap(int[] bitMap) {
        this.bitMap = bitMap;
    }

    public int[] getVirtualBitMap() {
        return virtualBitMap;
    }

    public void setVirtualBitMap(int[] virtualBitMap) {
        this.virtualBitMap = virtualBitMap;
    }

    private void initFile(Path path, int size, int[] map) throws IOException {
        map = new int[size];
        Arrays.fill(map, -1);

        BufferedWriter writer = Files.newBufferedWriter(path);
        writer.write(Arrays.toString(map));
    }
}