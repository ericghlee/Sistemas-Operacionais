import org.junit.jupiter.api.Test;
import org.testng.Assert;

class FileUtilsTest {

    @Test
    public void testReadFile() throws Exception {
        MemorySchedule memorySchedule = FileUtils.readFile("entryExample");

        Assert.assertEquals(40, memorySchedule.getTotal());
        Assert.assertEquals(20, memorySchedule.getVirtual());
        Assert.assertEquals(4, memorySchedule.getSize());
        Assert.assertEquals(1, memorySchedule.getPage());

        Instruction instruction;

        instruction = memorySchedule.getNextInstruction();
        Assert.assertTrue(instruction instanceof ProcessInstruction);

        ProcessInstruction processInstruction = (ProcessInstruction) instruction;

        Assert.assertEquals(1, processInstruction.getStart());
        Assert.assertEquals(5, processInstruction.getEnd());
        Assert.assertEquals(8, processInstruction.getMemory());
        Assert.assertEquals("nome", processInstruction.getName());

        int positions[] = {1, 3, 5};
        int times[] = {2, 4, 6};

        for (int i = 0; i < times.length; i++) {
            ProcessInstruction.Access access = processInstruction.getNextAccess();
            Assert.assertEquals(positions[i], access.getPosition());
            Assert.assertEquals(times[i], access.getTime());
        }

        instruction = memorySchedule.getNextInstruction();
        Assert.assertTrue(instruction instanceof CompactInstruction);
        Assert.assertEquals(20, ((CompactInstruction) instruction).getStart());

    }

    @Test
    public void testReadFirstLine() {
        String line = "1 2 3 4";

        MemorySchedule result = FileUtils.readFirstLine(line);

        Assert.assertEquals(1, result.getTotal());
        Assert.assertEquals(2, result.getVirtual());
        Assert.assertEquals(3, result.getSize());
        Assert.assertEquals(4, result.getPage());
    }

    @Test
    public void testReadCompactLine() {
        String line = "20 COMPACTAR";

        Instruction result = FileUtils.readLine(line);

        Assert.assertTrue(result instanceof CompactInstruction);
        Assert.assertEquals(20, ((CompactInstruction) result).getStart());
    }

    @Test
    public void testReadProcessLine() {
        String line = "1 5 8 nome 1 2 3 4 5 6";

        Instruction result = FileUtils.readLine(line);

        Assert.assertTrue(result instanceof ProcessInstruction);

        ProcessInstruction processInstruction = (ProcessInstruction) result;

        Assert.assertEquals(1, processInstruction.getStart());
        Assert.assertEquals(5, processInstruction.getEnd());
        Assert.assertEquals(8, processInstruction.getMemory());
        Assert.assertEquals("nome", processInstruction.getName());

        int positions[] = {1, 3, 5};
        int times[] = {2, 4, 6};

        for (int i = 0; i < times.length; i++) {
            ProcessInstruction.Access access = processInstruction.getNextAccess();
            Assert.assertEquals(positions[i], access.getPosition());
            Assert.assertEquals(times[i], access.getTime());
        }
    }
}