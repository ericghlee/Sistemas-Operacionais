import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public abstract class PagingStrategy {

    int pageSize;

    int maxActive;

    Map<Integer, Page> pages = new HashMap<>();

    List<Page> activePages;

    Page pageToUpdate;

    public PagingStrategy(int pageSize) {
        this.pageSize = pageSize;
    }

    public Page getPage(int position) {
        int pageIndex = position/pageSize;
        Page page = pages.get(pageIndex);

        if (activePages.contains(page)) {
            return page;
        } else {
            return switchPage(page);
        }
    }

    public void setup(MemoryScheduler memoryScheduler) {
        maxActive = memoryScheduler.getTotal()/memoryScheduler.getPage();

        int counter = memoryScheduler.getVirtual();
        int totalPages = memoryScheduler.getVirtual()/memoryScheduler.getPage();

        for(int i = 0; i < totalPages; i++) {
            int[] memory;

            if (counter > pageSize) {
                memory = new int[pageSize];
                counter -= pageSize;
            } else {
                memory = new int[counter];
            }

            Arrays.fill(memory, -1);
            pages.put(i, new Page(i, memory));
        }

        initActivePages();
    }

    public abstract Page switchPage(Page page);

    public abstract Page nextPage();

    public abstract void initActivePages();

    public Page getPageToUpdate() {
        return pageToUpdate;
    }

    public void setPageToUpdate(Page pageToUpdate) {
        this.pageToUpdate = pageToUpdate;
    }

    public int getPageSize() {
        return pageSize;
    }
}