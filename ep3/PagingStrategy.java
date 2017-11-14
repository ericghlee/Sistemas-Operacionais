public abstract class PagingStrategy {

    private int pageSize;

    private int[][] pages;

    public PagingStrategy(int pageSize, int virtualMemory) {
        this.pageSize = pageSize;

    }
}
