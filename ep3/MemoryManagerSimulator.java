import java.io.IOException;
import java.util.Scanner;

public class MemoryManagerSimulator {

    private static MemorySchedule memorySchedule;
    private static SpaceManager spaceManager;
    private static PagingStrategy pagingStrategy;

    public static void main(String[] args) {
        try {
            Scanner reader = new Scanner(System.in);
            String entry, command;
            String[] entrys;

            while (true) {
                System.out.print(">>: ");
                entry = reader.nextLine();

                entrys = entry.split(" ");

                switch (entrys[0]) {
                    case "carrega":
                        memorySchedule = FileUtils.readFile(entrys[1]);
                        break;

                    case "espaco":
                        break;

                    case "substitui":
                        break;

                    case "executa":
                        break;

                    case "sai":
                        break;
                }

            }

        } catch (IOException e) {
            System.out.println("Falha ao ler o arquivo.");
        }
    }

    private void readOptions() {

    }


}