import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;

public abstract class PagingStrategy {

    int pageSize;

    List<Page> pages = new ArrayList<>();

    List<Page> activePages;

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

    public List<Page> getPages() {
        return pages;
    }
}