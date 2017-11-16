import java.io.IOException;

public class Executor {
    private final MemoryScheduler memoryScheduler;

    private final SpaceManager spaceManager;

    private final PagingStrategy pagingStrategy;

    public Executor(MemoryScheduler memoryScheduler, SpaceManager spaceManager, PagingStrategy pagingStrategy) {
        this.memoryScheduler = memoryScheduler;
        this.spaceManager = spaceManager;
        this.pagingStrategy = pagingStrategy;
    }

    public void execute() {
        try {
            Bitmap bitmap = new Bitmap(memoryScheduler.getSize(), memoryScheduler.getVirtual());

            while (memoryScheduler.haveNext()) {
                Instruction instruction = memoryScheduler.getNextInstruction();

                if (instruction instanceof ProcessInstruction) {
                    Page page = pagingStrategy.getPage(bitmap);



                } else {

                }
            }


        } catch (IOException e) {
            System.out.println("ERRO: Falha na geração do bitmap");
        }

    }

    private void processInstruction(ProcessInstruction instruction) {

    }
}
