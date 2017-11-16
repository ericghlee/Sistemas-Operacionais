import java.io.IOException;
import java.util.Scanner;

public class MemoryManagerSimulator {

    private static MemoryScheduler memoryScheduler;
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
                        memoryScheduler = FileUtils.readFile(entrys[1]);
                        break;

                    case "espaco":
                        switch (Integer.valueOf(entrys[1])) {
                            case 1:
                                spaceManager = new BestFit();
                                break;
                            case 2:
                                break;
                            case 3:
                                break;
                        }

                        break;

                    case "substitui":
                        switch (Integer.valueOf(entrys[1])) {
                            case 1:
                                break;
                            case 2:
                                break;
                            case 3:
                                break;
                        }
                        break;

                    case "executa":
                        Executor executor = new Executor(memoryScheduler, spaceManager, pagingStrategy);
                        executor.execute();

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