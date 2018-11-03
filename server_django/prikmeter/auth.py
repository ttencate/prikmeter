from django.contrib.auth import hashers
from django.contrib.auth.models import User


class EmailAuthBackend:
    '''
    Authenticates the user by email address, instead of by username.
    https://docs.djangoproject.com/en/2.1/topics/auth/customizing/#writing-an-authentication-backend
    '''

    def get_user(self, user_id):
        try:
            return User.objects.get(email=user_id)
        except User.DoesNotExist:
            return None

    def authenticate(self, request, email=None, password=None):
        user = self.get_user(email)
        if not user:
            return None
        if hashers.check_password(password, user.password):
            return user
        else:
            return None
