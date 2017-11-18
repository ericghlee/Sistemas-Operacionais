public class Page {

    private final int pageIndex;

    private int[] memory;

    private boolean isChanged;

    private int start;

    private int end;

    public Page(int pageIndex, int[] memory) {
        this.pageIndex = pageIndex;
        this.memory = memory;
        this.isChanged = false;
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

    public void setChanged() {
        isChanged = true;
    }

    public void setNotChanged() {
        isChanged = false;
    }

    public boolean isChanged() {
        return isChanged;
    }

    public int getStart() {
        return start;
    }

    public void setStart(int start) {
        this.start = start;
    }

    public int getEnd() {
        return end;
    }

    public void setEnd(int end) {
        this.end = end;
    }
}