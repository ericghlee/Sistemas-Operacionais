import java.util.Arrays;
import java.util.LinkedList;

public class FIFOStrategy extends PagingStrategy {

    public FIFOStrategy(int pageSize, int virtualMemory) {
        super(pageSize, virtualMemory);
        activePages = new LinkedList<>();

        int counter = virtualMemory;
        int totalPages = virtualMemory/pageSize;
        for(int i = 0; i < totalPages; i++) {
            int[] memory;

            if (counter > pageSize) {
                memory = new int[pageSize];
                counter -= pageSize;
            } else {
                memory = new int[counter];
            }

            Arrays.fill(memory, -1);
            pages.add(new Page(i, memory));
        }
    }

    @Override
    public void switchPage(Page page) {
        if (activePages.isEmpty()) {
            ((LinkedList) activePages).addLast(page);
        } else {
            page = ((LinkedList<Page>) activePages).pollFirst();
        }
    }

    @Override
    public Page nextPage() {
        return null;
    }
}