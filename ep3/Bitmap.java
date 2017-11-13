import java.nio.file.Path;
import java.nio.file.Paths;

public class Bitmap {

    final Path file;

    public Bitmap(String path) {
        this.file = Paths.get(path);

    }


}
