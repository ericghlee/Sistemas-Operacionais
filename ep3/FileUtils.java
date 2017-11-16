import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;

public class FileUtils {

    public static MemoryScheduler readFile(String fileName) throws IOException {

        String firstLine = Files.lines(Paths.get(fileName)).findFirst().get();

        MemoryScheduler memoryScheduler = readFirstLine(firstLine);

        Files.lines(Paths.get(fileName)).filter(x -> !x.equals(firstLine))
                .forEachOrdered(x -> memoryScheduler.addInstruction(readLine(x)));

        return memoryScheduler;
    }

    public static Instruction readLine(String line) {
        String[] lineData = line.split(" ");

        if (lineData[1].equals("COMPACTAR")) {
            return new CompactInstruction(Integer.valueOf(lineData[0]));
        } else {
            return new ProcessInstruction(lineData);
        }
    }

    public static MemoryScheduler readFirstLine(String firstLine) {
        final String[] firstLineData = firstLine.split(" ");

        final int total = Integer.valueOf(firstLineData[0]);

        final int virtual = Integer.valueOf(firstLineData[1]);

        final int size = Integer.valueOf(firstLineData[2]);

        final int page = Integer.valueOf(firstLineData[3]);

        return new MemoryScheduler(total, virtual, size, page);
    }
}