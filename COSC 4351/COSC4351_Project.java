import java.util.Scanner;

public class COSC4351_Project {

    final static String[] DAYS_OF_WEEK = {
            "Saturday", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday",
            "Friday"
        };

    public static boolean isValidDate(int year, int month, int day){
        if(month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12){
            if(day > 31 || day < 1){ return false; }
        }
        else if(month == 2){
            if((year % 400 == 0) || ((year % 4 == 0) && (year % 100 != 0))){
                if(day > 29 || day < 1) { return false; }
            }
            else {
                if(day > 28 || day < 1) { return false; }
            }
        }
        else{
            if(day > 30 || day < 1) { return false; }
        }
        return true;
    }

    public static String findDayOfWeek(int year, int month, int day){
        if(month < 3){
            month += 12;
            year -= 1;
        }

        int century = year / 100;
        int centuryYear = year % 100;

        int dayOfWeek = (day + ((13 * (month + 1))/5) + centuryYear + (centuryYear / 4) + (century / 4) + (5 * century)) % 7;

        return DAYS_OF_WEEK[dayOfWeek];
    }

    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);

        System.out.print("Enter year of birth (Example: 2017): ");
        int year = input.nextInt();
        while(year < 1){
            System.out.println("Invalid year.");
            System.out.print("Enter a year greater than or equal to 1: ");
            year = input.nextInt();
        }

        System.out.print("Enter month of birth as a number between 1 and 12: ");
        int month = input.nextInt();
        while(month < 1 || month > 12){
            System.out.println("Invalid month.");
            System.out.print("Enter month of birth as a number between 1 and 12: ");
            month = input.nextInt();
        }

        System.out.print("Enter day of birth (Example: 14 if February 14): ");
        int day = input.nextInt();
        while(!isValidDate(year, month, day)){
            System.out.println("Invalid day.");
            System.out.print("Enter a valid day: ");
            day = input.nextInt();
        }

        String dayOfWeek = findDayOfWeek(year, month, day);

        System.out.println("That date was a " + dayOfWeek + ".");
    }
}
