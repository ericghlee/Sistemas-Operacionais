import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;

public abstract class PagingStrategy {

    private int pageSize;

    private List<Page> pages = new ArrayList<>();

    private LinkedList<Page> activePages;

    public PagingStrategy(int pageSize, int virtualMemory) {
        this.pageSize = pageSize;
    }

    public Page getPage(Bitmap bitmap, int processId) {
        for (Page page : pages) {
            if (page.getProcessId() == processId) {
                if (!activePages.contains(page)) {
                    switchPage(page);
                }

                return page;
            }
        }

        return nextPage();
    }

    public abstract void switchPage(Page page);

    public abstract Page nextPage();
}