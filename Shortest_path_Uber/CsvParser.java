import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.FileReader;
import java.io.BufferedReader;
import java.util.Collection;
import java.util.ArrayList;
//import java.util.Arrays;
//import java.util.List;
//import java.util.Scanner;

public class CsvParser {

    public static String[][] readCSV(String path) throws FileNotFoundException, IOException {
        try (FileReader fr = new FileReader(path); BufferedReader br = new BufferedReader(fr)) {
            Collection<String[]> lines = new ArrayList<>();
            for (String line = br.readLine(); line != null; line = br.readLine()) {
                lines.add(line.split(","));
            }
            return lines.toArray(new String[lines.size()][]);
        } catch(IOException e) {
            return new String[0][0];
        } 
    }

}