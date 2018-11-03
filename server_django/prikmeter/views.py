from django.contrib import auth, messages
from django.shortcuts import redirect, render
from django.views.decorators.http import require_POST, require_safe


@require_safe
def index(request):
    context = {}
    return render(request, 'prikmeter/index.html', context)


@require_POST
def login(request):
    email = request.POST['email']
    password = request.POST['password']
    user = auth.authenticate(request, email=email, password=password)
    if user:
        auth.login(request, user)
    else:
        messages.error(request, 'Invalid username or password.')
    return redirect(request.POST['next'] or 'prikmeter:index')


@require_POST
def logout(request):
    auth.logout()
    return redirect(request.POST['next'] or 'prikmeter:index')
