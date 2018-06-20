import org.junit.Assert;
import org.junit.Test;

public class COSC4351_Project_Unit_Tests {
    COSC4351_Project testClass;

    @Test
    public void testBirthday01(){
        Assert.assertEquals(testClass.findDayOfWeek(1991,10,15),"Tuesday");
    }

    @Test
    public void testBirthday02(){
        Assert.assertEquals(testClass.findDayOfWeek(2004,2,14), "Saturday");
    }

    @Test
    public void testBirthday03(){
        Assert.assertEquals(testClass.findDayOfWeek(2000,12,25), "Monday");
    }

    @Test
    public void testBirthday04(){
        Assert.assertEquals(testClass.findDayOfWeek(1933,9,12), "Tuesday");
    }

    @Test
    public void testBirthday05(){
        Assert.assertEquals(testClass.findDayOfWeek(2020,1,1), "Wednesday");
    }

    @Test
    public void testValidDate01(){
        Assert.assertEquals(testClass.isValidDate(2001,2,29), false);
    }

    @Test
    public void testValidDate02(){
        Assert.assertEquals(testClass.isValidDate(2000,2,29), true);
    }

    @Test
    public void testValidDate03(){
        Assert.assertEquals(testClass.isValidDate(2001,7,105), false);
    }

    @Test
    public void testValidDate04(){
        Assert.assertEquals(testClass.isValidDate(2001,5,-1), false);
    }

    @Test
    public void testValidDate05(){
        Assert.assertEquals(testClass.isValidDate(2001,4,31), false);
    }

    @Test
    public void testValidDate06(){
        Assert.assertEquals(testClass.isValidDate(2001,3,31), true);
    }
}
