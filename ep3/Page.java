public class Page {

    private final int pageIndex;

    private int[] memory;

    private int processId;

    public Page(int pageIndex, int[] memory) {
        this.pageIndex = pageIndex;
        this.memory = memory;
    }

    public int getPageIndex() {
        return pageIndex;
    }

    public int[] getMemory() {
        return memory;
    }

    public void setMemory(int[] memory) {
        this.memory = memory;
    }

    public int getProcessId() {
        return processId;
    }
}