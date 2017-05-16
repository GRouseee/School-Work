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
	
	public static void main(String[] args){
		try{
			getPassword();
		}catch(Exception e){
			System.out.println("Something bad happened...");
		}
	}
	
	static void getPassword() throws NoSuchAlgorithmException, InvalidKeySpecException{
		String passwordRegex = "(?=.*[0-9])(?=.*[a-z])(?=.*[A-Z])(?=.*[@#$%^&+=])(?=\\S+$).{8,}";
		boolean isValid = false;
		
		while(!isValid){
			System.out.println("Enter a password at least 8 characters long (must contain a lower case, upper case, a digit, and a special character): ");
			String passwordAttempt = kb.nextLine();
			isValid = true;
			
			if(compareToRegex(passwordRegex, passwordAttempt)){
				getSecuredPassword(passwordAttempt, getSalt());
				System.out.println("valid");//just to test
				return;
			}
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
}