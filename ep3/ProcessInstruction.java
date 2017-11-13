import java.util.LinkedList;

public class ProcessInstruction implements Instruction {

    private final int start;

    private final int end;

    private final int memory;

    private final String name;

    private final LinkedList<Access> memoryAccess = new LinkedList<>();

    public ProcessInstruction(String[] data) {
        this.start = Integer.valueOf(data[0]);
        this.end = Integer.valueOf(data[1]);
        this.memory = Integer.valueOf(data[2]);
        this.name = data[3];

        for (int i = 4; i < data.length; i+=2) {
            memoryAccess.addLast(new Access(Integer.valueOf(data[i]), Integer.valueOf(data[i+1])));
        }
    }

    public int getStart() {
        return start;
    }

    public int getEnd() {
        return end;
    }

    public int getMemory() {
        return memory;
    }

    public String getName() {
        return name;
    }

    public Access getNextAccess() {
        return memoryAccess.pop();
    }

    public class Access {
        private final int position;
        private final int time;

        public Access(int position, int time) {
            this.position = position;
            this.time = time;
        }

        public int getPosition() {
            return position;
        }

        public int getTime() {
            return time;
        }
    }
}