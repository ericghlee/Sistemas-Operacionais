import java.io.IOException;

public class Executor {
    private final MemoryScheduler memoryScheduler;

    private final SpaceManager spaceManager;

    private final PagingStrategy pagingStrategy;

    private Bitmap bitmap;

    public Executor(MemoryScheduler memoryScheduler, SpaceManager spaceManager, PagingStrategy pagingStrategy) {
        this.memoryScheduler = memoryScheduler;
        this.spaceManager = spaceManager;
        this.pagingStrategy = pagingStrategy;
    }

    public void execute() {
        try {
            pagingStrategy.setup(memoryScheduler);
            bitmap = new Bitmap(memoryScheduler.getSize(), memoryScheduler.getVirtual());

            while (memoryScheduler.haveNext()) {
                Instruction instruction = memoryScheduler.getNextInstruction();

                if (instruction instanceof ProcessInstruction) {
                    processInstruction((ProcessInstruction) instruction);
                } else {
                    processInstruction((CompactInstruction) instruction);
                }
            }


        } catch (IOException e) {
            System.out.println("ERRO: Falha na geração do bitmap");
        }

    }

    private void processInstruction(ProcessInstruction instruction) {
        int position = spaceManager.getPosition(instruction.getMemory(), bitmap.getVirtualBitMap());
        Page page = pagingStrategy.getPage(position);

        if (pagingStrategy.getPageToUpdate() != null) {
            bitmap.updateVirtual(page, memoryScheduler.getPage());
        }

    }

    private void processInstruction(CompactInstruction instruction) {

    }
}
