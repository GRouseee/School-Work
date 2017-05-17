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
	static Scanner kb = new Scanner(System.in);
	static String password = null;
	static int int1 = 0;
	static int int2 = 0;
	static int addResult = 0;
	static int multResult = 0;
	
	public static void main(String[] args){
		try{

			getPassword();
			getOutputName();


			getPassword();
			getInteger();
			getInteger();
			
			System.out.println(password);
			System.out.println(addResult);
			System.out.println(multResult);
		}catch(Exception e){
			System.out.println("Something bad happened...");
		}
	}
	
	static boolean getInput(String regex, String prompt, String inputType) throws NoSuchAlgorithmException, InvalidKeySpecException{
		boolean isValid = false;
		int intCount = 0;
		
		while(!isValid){
			System.out.println(prompt);

			String attempt = kb.nextLine();

			//isValid = true;

			
			if(inputType.equals("password") && compareToRegex(regex, attempt)){
				password = getSecuredPassword(attempt, getSalt()).toString();
				isValid = true;
			}
			else if(inputType.equals("outputfile") && compareToRegex(regex, attempt)){
				isValid = checkUniqueFileName(attempt);
			}
			else{
				System.out.println("Failed");
			}
			if(inputType.equals("integer") && compareToRegex(regex, attempt)){
				intCount++;
				int1 = Integer.parseInt(attempt);
				
				if(intCount == 2){
					int2 =Integer.parseInt(attempt);
					addResult = addInts(int1, int2);
					multResult = multInts(int1, int2);
					isValid = true;
				}
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
		byte[] salt = new byte[8];
		SecureRandom random = SecureRandom.getInstance("SHA1PRNG");
		random.nextBytes(salt);
		
		return salt;
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
		String prompt = "Enter a output file name. (must be within this directory, the file cannot exist, no file extensions, and it must be using the English Alphabet.)";
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

}
