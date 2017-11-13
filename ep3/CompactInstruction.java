public class CompactInstruction implements Instruction {

    private final int start;

    public CompactInstruction(int start) {
        this.start = start;
    }

    public int getStart() {
        return start;
    }
}