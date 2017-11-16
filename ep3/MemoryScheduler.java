import java.util.LinkedList;

public class MemoryScheduler {

    private final int total;

    private final int virtual;

    private final int size;

    private final int page;

    private final LinkedList<Instruction> instructions = new LinkedList<Instruction>();

    public MemoryScheduler(int total, int virtual, int size, int page) {
        this.total = total;
        this.virtual = virtual;
        this.size = size;
        this.page = page;
    }

    public int getTotal() {
        return total;
    }

    public int getVirtual() {
        return virtual;
    }

    public int getSize() {
        return size;
    }

    public int getPage() {
        return page;
    }

    public Instruction getNextInstruction() {
        return instructions.pop();
    }

    public boolean haveNext() {
        return !instructions.isEmpty();
    }

    public void addInstruction(Instruction instruction) {
        instructions.addLast(instruction);
    }
}