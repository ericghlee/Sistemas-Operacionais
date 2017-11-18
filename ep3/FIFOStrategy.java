import java.util.LinkedList;

public class FIFOStrategy extends PagingStrategy {

    public FIFOStrategy(int pageSize) {
        super(pageSize);
    }

    @Override
    public Page switchPage(Page page) {
        if (activePages.size() == maxActive) {
            Page candidate = ((LinkedList<Page>) activePages).pollFirst();
            page.setStart(candidate.getStart());
            page.setEnd(candidate.getEnd());

            if (candidate.isChanged()) {
                pageToUpdate = candidate;
            }
        } else {
            page.setStart(last);
            page.setEnd(page.getMemory().length);
        }

        ((LinkedList) activePages).addLast(page);

        return page;
    }

    @Override
    public Page nextPage() {
        return null;
    }

    @Override
    public void initActivePages() {
        activePages = new LinkedList<>();
    }
}