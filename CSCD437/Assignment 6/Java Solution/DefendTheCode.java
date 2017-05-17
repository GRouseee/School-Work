import java.io.*;
import java.security.NoSuchAlgorithmException;
import java.security.SecureRandom;
import java.security.spec.InvalidKeySpecException;
import java.security.spec.KeySpec;
import java.util.Arrays;
import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import javax.crypto.SecretKeyFactory;
import javax.crypto.spec.PBEKeySpec;

public class DefendTheCode{
	private static Scanner kb = new Scanner(System.in);
	private static int int1, int2, addResult, multResult;
	private static String firstName, lastName, inputFile, password, verifyPassword, outputFile;
	private static byte[] salt;

	public static void main(String[] args){
		try{
			getFullName();
			getPassword();
			verifyPassword();
			getInteger();
			getInputFile();
			getOutputName();
			if(combineContents()){
				System.out.println("Successfully copied First + Last name, addition of the integers, mastication" +
						" of the integers, and the contents of the input file into the output file which can be " +
						"found in the directory of the java file.");
			}
			System.out.println(firstName + " " + lastName);
			System.out.println(password);
			System.out.println(addResult);
			System.out.println(multResult);
     		System.out.println(inputFile.toString());
     		System.out.println(outputFile);
		}catch(Exception e){
			System.out.println("Something bad happened...");
			e.printStackTrace();
		}
	}
	
	static boolean getInput(String regex, String prompt, String inputType) throws NoSuchAlgorithmException, InvalidKeySpecException{
		boolean isValid = false;
		int intCount = 0;

		while(!isValid){
			System.out.println(prompt);

			String attempt = kb.nextLine();

			if(inputType.equals("password") && compareToRegex(regex, attempt)){
				password = getSecuredPassword(attempt, getSalt()).toString();
				isValid = true;
			}
			
			else if(inputType.equals("verifypassword") && compareToRegex(regex, attempt)){
				verifyPassword = getSecuredPassword(attempt, salt).toString();
				isValid = validatePassword(attempt, password.getBytes(), salt);
			}

			else if(inputType.equals("outputfile") && compareToRegex(regex, attempt)){
				isValid = checkUniqueFileName(attempt);
				outputFile = attempt;
			}

			else if(inputType.equals("integer") && compareToRegex(regex, attempt)){
				intCount++;
				int1 = Integer.parseInt(attempt);

				if(intCount == 2){
					int2 =Integer.parseInt(attempt);
					addResult = addInts(int1, int2);
					multResult = multInts(int1, int2);
					isValid = true;
				}
			}

			else if(inputType.equals("firstName") && compareToRegex(regex, attempt)){
				firstName = attempt;
				isValid = true;
			}

			else if(inputType.equals("lastName") && compareToRegex(regex, attempt)){
				lastName = attempt;
				isValid = true;
			}

			else if(inputType.equals("inputFile") && compareToRegex(regex, attempt)){
				inputFile = attempt;
				isValid = true;
			}
		}
		
		return isValid;
	}
	
	static int addInts(int num1, int num2){
		return num1 + num2;
	}

	static int multInts(int num1, int num2){
		return num1 * num2;
	}

	static boolean getInteger() throws NoSuchAlgorithmException, InvalidKeySpecException{
		String intRegex = "\\d{1}";
		String prompt = "Enter an integer value: ";
		boolean isValid = getInput(intRegex, prompt, "integer");

		return isValid;
	}

	static boolean getPassword() throws NoSuchAlgorithmException, InvalidKeySpecException{
		String passwordRegex = "(?=.*[0-9])(?=.*[a-z])(?=.*[A-Z])(?=.*[@#$%^&+=])(?=\\S+$).{8,}";
		String prompt = "Enter a password at least 8 characters long (must contain a lower case, upper case, a digit, and a special character): ";
		boolean isValid = getInput(passwordRegex, prompt, "password");
		
		return isValid;
	}
	
	static boolean verifyPassword() throws NoSuchAlgorithmException, InvalidKeySpecException{
		String passwordRegex = "(?=.*[0-9])(?=.*[a-z])(?=.*[A-Z])(?=.*[@#$%^&+=])(?=\\S+$).{8,}";
		String prompt = "Re-enter your password: ";
		boolean isValid = getInput(passwordRegex, prompt, "verifypassword");
		
		if(isValid){
			System.out.println("Your password is valid.");
			return isValid;
		}
		else{
			System.out.println("Password is invalid.");
			return isValid;
		}
	}
	
	static boolean validatePassword(String passwordAttempt, byte[] securePassword, byte[] salt) throws NoSuchAlgorithmException, InvalidKeySpecException{
		byte[] securePasswordAttempt = getSecuredPassword(passwordAttempt, salt);
		
		return Arrays.equals(securePassword, securePasswordAttempt);
	}
	
	static byte[] getSecuredPassword(String password, byte[] salt) throws NoSuchAlgorithmException, InvalidKeySpecException{
		/*Make our secure password*/
		int keyLen = 160;
		int iterations = 2000;
		KeySpec keySpec = new PBEKeySpec(password.toCharArray(), salt, iterations, keyLen);
		SecretKeyFactory key = SecretKeyFactory.getInstance("PBKDF2WithHmacSHA1");
		
		return key.generateSecret(keySpec).getEncoded();
	}
	
	static byte[] getSalt() throws NoSuchAlgorithmException{
		/*Make our salt*/
		salt = new byte[8];
		SecureRandom random = SecureRandom.getInstance("SHA1PRNG");
		random.nextBytes(salt);
		
		return salt;
	}

	static boolean getFullName() throws InvalidKeySpecException, NoSuchAlgorithmException {
		String nameRegex = "^[A-Za-z'-]{1,50}$";
		String prompt = "Enter your first name (50 or fewer characters)";
		getInput(nameRegex, prompt, "firstName");
		prompt = "Enter your last name (50 or fewer characters)";
		boolean isValid = getInput(nameRegex, prompt, "lastName");

		return isValid;
	}

	static boolean getInputFile() throws InvalidKeySpecException, NoSuchAlgorithmException {
		String inputFileRegex = "^[\\w_-]{1,50}(.txt)$";
		String prompt = "Enter an input file from current working directory (e.g. input.txt)";
		boolean isValid = getInput(inputFileRegex, prompt, "inputFile");

		return isValid;
	}

	static boolean compareToRegex(String regex, String input){
        	Pattern pattern = Pattern.compile(regex);
        	Matcher matcher = pattern.matcher(input);
        
        	return matcher.matches();
	}


	/**
	 * Retrieves the output file name from the user.
	 * @return boolean
	 */
	static boolean getOutputName() throws NoSuchAlgorithmException, InvalidKeySpecException{
		String outputfileRegex = "([a-zA-Z0-9!#$%&\\(\\)\\{\\}\\]\\[\\^_`~@; ]){1,255}";
		String prompt = "Enter a output file name. (must be within this directory (run from commandline), the file cannot exist" +
				", no file extensions, and it must be using the English Alphabet.)";
		
		return getInput(outputfileRegex, prompt, "outputfile");
	}


	/**
	 * Checks current directory's file/directory names against incoming param. Returns true if unique.
	 * @param attempt
	 * @return boolean
	 */
	static boolean checkUniqueFileName(String attempt){

		File current_dir = new File(System.getProperty("user.dir") + "/src/Java");
		File[] listOfFiles = current_dir.listFiles();
		if(listOfFiles!=null){
			for(File f: listOfFiles){
				if(f.getName().equals(attempt+".txt")){
					System.out.println("File with that name already exists.");
					return false;
				}
			}
		}

		return true;
	}

	/**
	 * Combines First and Last name, addition of int1 and int2, multiplication of int1 and int2,
	 * and the contents of inputFile and writes them into the specified outputFile.
	 *
	 * Returns true if a successful combination, false otherwise.
	 * @return boolean
	 */
	static boolean combineContents(){

		try{
			String line;

			//Opens inputfile and creates output file
			File input = new File(inputFile);
			File output = new File(outputFile +".txt");

			//Opens inputfile reader
			FileReader fr = new FileReader(System.getProperty("user.dir")+ "/src/Java/"+inputFile);
			BufferedReader br = new BufferedReader(fr);

			//Opens outputfile Writer
			FileWriter fw = new FileWriter(outputFile +".txt");
			BufferedWriter bw = new BufferedWriter(fw);

			bw.write("First and Last name: "+ firstName + " " + lastName);
			bw.newLine();
			bw.write("Adding the two ints: "+ Integer.toString(addInts(int1, int2)));
			bw.newLine();
			bw.write("Multiplying the two ints: " + Integer.toString(multInts(int1,int2)));
			bw.newLine();

			while((line = br.readLine())!=null){
				bw.write(line);
				bw.newLine();
			}

			br.close();
			bw.close();
			fr.close();
			fw.close();
		}catch(Exception e){
			e.printStackTrace();
			return false;
		}


		return true;
	}

}